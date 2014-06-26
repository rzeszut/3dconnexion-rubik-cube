#include <cmath>
#include <cfloat>
#include <algorithm>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "cube/cube.h"

namespace cube {

static constexpr float ALMOST_ZERO = 1e-6;

Cube::~Cube() {
    for (auto &it : cubePieces) {
        if (it) {
            delete it;
        }
    }
}

void Cube::init(){
    for (int x : {-1, 0, 1}) {
        for (int y : {-1, 0, 1}) {
            for (int z : {-1, 0, 1}) {
                getPiece(x, y, z) = new CubePiece(x, y, z);
            }
        }
    }
}

void Cube::render() {
    for (int x : {-1, 0, 1}) {
        for (int y : {-1, 0, 1}) {
            for (int z : {-1, 0, 1}) {
                getPiece(x, y, z)->draw(x, y, z);
            }
        }
    }
}

void Cube::rotate(glm::mat4 projection, glm::mat4 modelView, glm::vec4 viewport,
                   glm::vec2 ptMouseWnd, glm::vec2 ptLastMouseWnd) {

    int windowWidth = viewport.w;
    glm::vec2 ptMouse(ptMouseWnd.x, windowWidth - ptMouseWnd.y);
    glm::vec2 ptLastMouse(ptLastMouseWnd.x, windowWidth - ptLastMouseWnd.y);

    // cube corners in window coordinates
    glm::vec3 cubeCorner[8] = {
        glm::project(glm::vec3(1.5, 1.5, 1.5),  modelView, projection, viewport),
        glm::project(glm::vec3(1.5,-1.5, 1.5),  modelView, projection, viewport),
        glm::project(glm::vec3(-1.5,-1.5, 1.5), modelView, projection, viewport),
        glm::project(glm::vec3(-1.5, 1.5, 1.5), modelView, projection, viewport),

        glm::project(glm::vec3(1.5, 1.5,-1.5),  modelView, projection, viewport),
        glm::project(glm::vec3(1.5,-1.5,-1.5),  modelView, projection, viewport),
        glm::project(glm::vec3(-1.5,-1.5,-1.5), modelView, projection, viewport),
        glm::project(glm::vec3(-1.5, 1.5,-1.5), modelView, projection, viewport)
    };

    // Find the min/max X and Y to do a rough bounding box test.
    float xMin = FLT_MAX, yMin = FLT_MAX;
    float xMax = FLT_MIN, yMax = FLT_MIN;
    for (int i = 0; i < 8; i++) {
        xMin = std::fmin(xMin, cubeCorner[i].x);
        yMin = std::fmin(yMin, cubeCorner[i].y);
        xMax = std::fmax(xMax, cubeCorner[i].x);
        yMax = std::fmax(yMax, cubeCorner[i].y);
    }

    // Check if point was outside rough test and return if it was.
    if (!(xMin <= ptLastMouse.x && ptLastMouse.x <= xMax
          && yMin <= ptLastMouse.y && ptLastMouse.y <= yMax)) {
        //Failed rough bounding box test
        return;
    }

    // corners for sides
    std::map<Side, std::array<glm::vec3, 4>> corners {
        {Side::RIGHT, {{cubeCorner[5], cubeCorner[1], cubeCorner[0], cubeCorner[4]}}},
        {Side::LEFT,  {{cubeCorner[6], cubeCorner[2], cubeCorner[3], cubeCorner[7]}}},
        {Side::TOP,   {{cubeCorner[7], cubeCorner[4], cubeCorner[0], cubeCorner[3]}}},
        {Side::BOTTOM,{{cubeCorner[6], cubeCorner[5], cubeCorner[1], cubeCorner[2]}}},
        {Side::FRONT, {{cubeCorner[2], cubeCorner[1], cubeCorner[0], cubeCorner[3]}}},
        {Side::BACK,  {{cubeCorner[6], cubeCorner[5], cubeCorner[4], cubeCorner[7]}}}
    };

    float fMinZ = FLT_MAX;
    float fTmp;
    optional::Optional<Side> sideFound;

    // test which side was hit
    for (auto &it : corners) {
        auto side = it.first;
        auto c = it.second;

        if (poly4InsideTest(c.data(), ptLastMouse.x, ptLastMouse.y)) {
            auto zCorners = {c[0].z, c[1].z, c[2].z, c[3].z};
            fTmp = *std::min_element(zCorners.begin(), zCorners.end());

            // choose side with min z
            if (fTmp < fMinZ) {
                sideFound = optional::Optional<Side>(side);
                fMinZ = fTmp;
            }
        }
    }

    if (!sideFound) {
        // Missed all the sides.
        return;
    }
    side = *sideFound;

    glm::vec2 vX(corners[side][1].x - corners[side][0].x,
                 corners[side][1].y - corners[side][0].y
                );

    glm::vec2 vY(corners[side][2].x - corners[side][1].x,
                 corners[side][2].y - corners[side][1].y
                );

    glm::vec2 vMouse(ptMouse.x - ptLastMouse.x,
                     ptMouse.y - ptLastMouse.y
                    );

    // angle between the mouse vector and the
    // X/Y vector for this cube side.
    vX = glm::normalize(vX);
    vY = glm::normalize(vY);
    vMouse = glm::normalize(vMouse);

    float xDiff = glm::angle(vX, vMouse);
    float yDiff = glm::angle(vY, vMouse);

    std::vector<float> diffs = {
        std::fabs(xDiff),
        std::fabs(xDiff - 180),
        std::fabs(yDiff),
        std::fabs(yDiff - 180)
    };
    float minDiff = *std::min_element(diffs.begin(), diffs.end()) + ALMOST_ZERO;

    // save rotation data
    rotating = true;
    rotationIteration = 0;

    if (diffs[0] <= minDiff) {
        rotation = Rotation::Y_ROTATION;
        clockwise = Clockwise::CLOCKWISE;
        section = getYsection(corners[side].data(), ptLastMouse.x ,ptLastMouse.y);
    }
    else if (diffs[1] <= minDiff) {
        rotation = Rotation::Y_ROTATION;
        clockwise = Clockwise::COUNTER_CLOCKWISE;
        section = getYsection(corners[side].data(), ptLastMouse.x ,ptLastMouse.y);
    }
    else if (diffs[2] <= minDiff) {
        rotation = Rotation::X_ROTATION;
        clockwise = Clockwise::CLOCKWISE;
        section = getXsection(corners[side].data(), ptLastMouse.x, ptLastMouse.y);
    }
    else if (diffs[3] <= minDiff) {
        rotation = Rotation::X_ROTATION;
        clockwise = Clockwise::COUNTER_CLOCKWISE;
        section = getXsection(corners[side].data(), ptLastMouse.x, ptLastMouse.y);
    } else {
        rotating = false;
    }
}

void Cube::update() {
    if (!rotating) {
        return;
    }

    switch (rotation) {
    case Rotation::X_ROTATION:
        updateXRotation();
        break;

    case Rotation::Y_ROTATION:
        updateYRotation();
        break;
    }
}

void Cube::updateXRotation() {
    switch (side) {
    case Side::FRONT:
    case Side::BOTTOM:
        updateXSection(invert(clockwise));
        break;

    case Side::BACK:
    case Side::TOP:
        updateXSection(clockwise);
        break;

    case Side::LEFT:
        updateZSection(invert(clockwise));
        break;

    case Side::RIGHT:
        updateZSection(clockwise);
        break;
    }
}

void Cube::updateYRotation() {
    switch (side) {
    case Side::FRONT:
    case Side::LEFT:
        updateYSection(clockwise);
        break;

    case Side::BACK:
    case Side::RIGHT:
        updateYSection(invert(clockwise));
        break;

    case Side::TOP:
        updateZSection(invert(clockwise));
        break;

    case Side::BOTTOM:
        updateZSection(clockwise);
        break;
    }
}

void Cube::updateXSection(Clockwise modifier) {
    static glm::vec3 rotationAxis(1, 0, 0);

    // get pieces for the section moved
    CubePiece* pieces[] = {
        getPiece(section, 1, 1),
        getPiece(section,-1, 1),
        getPiece(section,-1,-1),
        getPiece(section, 1,-1),
        getPiece(section, 1, 0),
        getPiece(section, 0, 1),
        getPiece(section,-1, 0),
        getPiece(section, 0,-1),
        getPiece(section, 0, 0),
    };

    // animate
    if (animate(pieces, rotationAxis, modifier)) {
        return;
    }
    rotating = false;

    // after animating the move, clear rotation angle and vector
    // and rotate sides of all pieces in the section
    for (int i = 0; i < 9; i++) {
        pieces[i]->clrRotation();
        pieces[i]->rotateX((bool) modifier);
    }

    // swap pieces in the section
    CubePiece* pTmp;
    if ((bool) modifier) {
        pTmp = getPiece(section, 1, 1);
        getPiece(section, 1, 1) = getPiece(section, 1,-1);
        getPiece(section, 1,-1) = getPiece(section,-1,-1);
        getPiece(section,-1,-1) = getPiece(section,-1, 1);
        getPiece(section,-1, 1) = pTmp;

        pTmp = getPiece(section, 1, 0);
        getPiece(section, 1, 0) = getPiece(section, 0,-1);
        getPiece(section, 0,-1) = getPiece(section,-1, 0);
        getPiece(section,-1, 0) = getPiece(section, 0, 1);
        getPiece(section, 0, 1) = pTmp;
    } else {
        pTmp = getPiece(section, 1, 1);
        getPiece(section, 1, 1) = getPiece(section,-1, 1);
        getPiece(section,-1, 1) = getPiece(section,-1,-1);
        getPiece(section,-1,-1) = getPiece(section, 1,-1);
        getPiece(section, 1,-1) = pTmp;

        pTmp = getPiece(section, 1, 0);
        getPiece(section, 1, 0) = getPiece(section, 0, 1);
        getPiece(section, 0, 1) = getPiece(section,-1, 0);
        getPiece(section,-1, 0) = getPiece(section, 0,-1);
        getPiece(section, 0,-1) = pTmp;
    }
}

void Cube::updateYSection(Clockwise modifier) {
    static glm::vec3 rotationAxis(0, 1, 0);

    // get pieces for the section moved
    CubePiece* pieces[] = {
        getPiece( 1, section, 1),
        getPiece(-1, section, 1),
        getPiece(-1, section,-1),
        getPiece( 1, section,-1),
        getPiece( 1, section, 0),
        getPiece( 0, section, 1),
        getPiece(-1, section, 0),
        getPiece( 0, section,-1),
        getPiece( 0, section, 0)
    };

    // animate
    if (animate(pieces, rotationAxis, modifier)) {
        return;
    }
    rotating = false;

    // after animating the move, clear rotation angle and vector
    // and rotate sides of all pieces in the section
    for (int i = 0; i < 9; i++) {
        pieces[i]->clrRotation();
        pieces[i]->rotateY((bool) modifier);
    }

    // swap pieces in the section
    CubePiece* pTmp;
    if ((bool) modifier) {
        pTmp = getPiece(1, section, 1);
        getPiece( 1, section, 1) = getPiece(-1, section, 1);
        getPiece(-1, section, 1) = getPiece(-1, section,-1);
        getPiece(-1, section,-1) = getPiece( 1, section,-1);
        getPiece( 1, section,-1) = pTmp;

        pTmp = getPiece(1, section, 0);
        getPiece( 1, section, 0) = getPiece( 0, section, 1);
        getPiece( 0, section, 1) = getPiece(-1, section, 0);
        getPiece(-1, section, 0) = getPiece( 0, section,-1);
        getPiece( 0, section,-1) = pTmp;
    }
    else {
        pTmp = getPiece(1, section, 1);
        getPiece( 1, section, 1) = getPiece( 1, section,-1);
        getPiece( 1, section,-1) = getPiece(-1, section,-1);
        getPiece(-1, section,-1) = getPiece(-1, section, 1);
        getPiece(-1, section, 1) = pTmp;

        pTmp = getPiece(1, section, 0);
        getPiece( 1, section, 0) = getPiece( 0, section,-1);
        getPiece( 0, section,-1) = getPiece(-1, section, 0);
        getPiece(-1, section, 0) = getPiece( 0, section, 1);
        getPiece( 0, section, 1) = pTmp;
    }

}

void Cube::updateZSection(Clockwise modifier) {
    static glm::vec3 rotationAxis(0, 0, 1);

    // get pieces for the section moved
    CubePiece* pieces[] = {
        getPiece( 1, 1, section),
        getPiece(-1, 1, section),
        getPiece(-1,-1, section),
        getPiece( 1,-1, section),
        getPiece( 1, 0, section),
        getPiece( 0, 1, section),
        getPiece(-1, 0, section),
        getPiece( 0,-1, section),
        getPiece( 0, 0, section)
    };

    // animate
    if (animate(pieces, rotationAxis, modifier)) {
        return;
    }
    rotating = false;

    // after animating the move, clear rotation angle and vector
    // and rotate sides of all pieces in the section
    for (int i = 0; i < 9; i++) {
        pieces[i]->clrRotation();
        pieces[i]->rotateZ((bool) modifier);
    }

    // swap pieces in the section
    CubePiece* pTmp;
    if ((bool) modifier) {
        pTmp = getPiece( 1, 1, section);
        getPiece( 1, 1, section) = getPiece( 1,-1, section);
        getPiece( 1,-1, section) = getPiece(-1,-1, section);
        getPiece(-1,-1, section) = getPiece(-1, 1, section);
        getPiece(-1, 1, section) = pTmp;

        pTmp = getPiece( 1, 0, section);
        getPiece( 1, 0, section) = getPiece( 0,-1, section);
        getPiece( 0,-1, section) = getPiece(-1, 0, section);
        getPiece(-1, 0, section) = getPiece( 0, 1, section);
        getPiece( 0, 1, section) = pTmp;
    }
    else {
        pTmp = getPiece( 1, 1, section);
        getPiece( 1, 1, section) = getPiece(-1, 1, section);
        getPiece(-1, 1, section) = getPiece(-1,-1, section);
        getPiece(-1,-1, section) = getPiece( 1,-1, section);
        getPiece( 1,-1, section) = pTmp;

        pTmp = getPiece( 1, 0, section);
        getPiece( 1, 0, section) = getPiece( 0, 1, section);
        getPiece( 0, 1, section) = getPiece(-1, 0, section);
        getPiece(-1, 0, section) = getPiece( 0,-1, section);
        getPiece( 0,-1, section) = pTmp;
    }
}

bool Cube::animate(CubePiece *pieces[9], const glm::vec3 &axis, Clockwise modifier) {
    if (rotationIteration < ROTATION_FRAMES) {
        float maxAngle = (bool) modifier ? 90.f : -90.f;
        float rotAngle = maxAngle * rotationIteration / (float) ROTATION_FRAMES;

        for (int i = 0; i < 9; i++) {
            pieces[i]->setRotation(rotAngle, axis);
        }

        ++rotationIteration;

        return true;
    }

    return false;
}

}

