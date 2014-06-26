#include <cmath>
#include <cfloat>
#include <algorithm>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "cube/cube.h"
#include "logging/logging.hpp"

namespace cube {

static constexpr float ALMOST_ZERO = 1e-6;

// for debug purposes
std::ostream &operator<<(std::ostream &stream, Rotation r) {
    switch (r) {
    case Rotation::X_ROTATION:
        stream << "X_ROTATION";
        break;

    case Rotation::Y_ROTATION:
        stream << "Y_ROTATION";
        break;
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, Clockwise c) {
    switch (c) {
    case Clockwise::Clockwise:
        stream << "Clockwise";
        break;

    case Clockwise::CounterClockwise:
        stream << "CounterClockwise";
        break;
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, SIDE s) {
    switch (s) {
    case SD_RIGHT:
        stream << "RIGHT";
        break;
    case SD_LEFT:
        stream << "LEFT";
        break;
    case SD_TOP:
        stream << "TOP";
        break;
    case SD_BOTTOM:
        stream << "BOTTOM";
        break;
    case SD_FRONT:
        stream << "FRONT";
        break;
    case SD_BACK:
        stream << "BACK";
        break;
    }
    return stream;
}
// end debug

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
                getPiece(x, y, z) = new CubePiece(glm::ivec3(x, y, z));
            }
        }
    }
}

void Cube::draw() {
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

    glm::vec3 corners[6][4] = {
        {cubeCorner[5], cubeCorner[1], cubeCorner[0], cubeCorner[4]},    //Right
        {cubeCorner[6], cubeCorner[2], cubeCorner[3], cubeCorner[7]},    //Left
        {cubeCorner[7], cubeCorner[4], cubeCorner[0], cubeCorner[3]},    //Top
        {cubeCorner[6], cubeCorner[5], cubeCorner[1], cubeCorner[2]},    //Bottom
        {cubeCorner[2], cubeCorner[1], cubeCorner[0], cubeCorner[3]},    //Front
        {cubeCorner[6], cubeCorner[5], cubeCorner[4], cubeCorner[7]},    //Back
    };

    float fMinZ = FLT_MAX;
    float fTmp;
    SIDE side = (SIDE) - 1;

    // test which side was hit
    for (int i = 0; i < 6; i++) {
        if (poly4InsideTest(corners[i], ptLastMouse.x, ptLastMouse.y)) {
            auto zCorners = {corners[i][0].z, corners[i][1].z, corners[i][2].z, corners[i][3].z};
            fTmp = *std::min_element(zCorners.begin(), zCorners.end());

            if (fTmp < fMinZ) {
                side = (SIDE)i;
                fMinZ = fTmp;
            }
        }
    }

    if ((int) side == -1) {
        // Missed all the sides.
        return;
    }

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
    this->rotating = true;
    this->rotationIteration = 0;
    this->side = side;

    if (diffs[0] <= minDiff) {
        this->rotation = Rotation::Y_ROTATION;
        this->clockwise = Clockwise::Clockwise;
        this->section = getYsection(corners[side], ptLastMouse.x ,ptLastMouse.y);
    }
    else if (diffs[1] <= minDiff) {
        this->rotation = Rotation::Y_ROTATION;
        this->clockwise = Clockwise::CounterClockwise;
        this->section = getYsection(corners[side], ptLastMouse.x ,ptLastMouse.y);
    }
    else if (diffs[2] <= minDiff) {
        this->rotation = Rotation::X_ROTATION;
        this->clockwise = Clockwise::Clockwise;
        this->section = getXsection(corners[side], ptLastMouse.x, ptLastMouse.y);
    }
    else if (diffs[3] <= minDiff) {
        this->rotation = Rotation::X_ROTATION;
        this->clockwise = Clockwise::CounterClockwise;
        this->section = getXsection(corners[side], ptLastMouse.x, ptLastMouse.y);
    } else {
        this->rotating = false;
    }

    LOG(DEBUG) << "Rotation: " << rotation
        << ", clockwise: " << clockwise
        << ", side: " << side
        << ", section: " << section;
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
    case SD_FRONT:
    case SD_BOTTOM:
        updateXSection(invert(clockwise));
        break;

    case SD_BACK:
    case SD_TOP:
        updateXSection(clockwise);
        break;

    case SD_LEFT:
        updateZSection(invert(clockwise));
        break;

    case SD_RIGHT:
        updateZSection(clockwise);
        break;
    }
}

void Cube::updateYRotation() {
    switch (side) {
    case SD_FRONT:
    case SD_LEFT:
        updateYSection(clockwise);
        break;

    case SD_BACK:
    case SD_RIGHT:
        updateYSection(invert(clockwise));
        break;

    case SD_TOP:
        updateZSection(invert(clockwise));
        break;

    case SD_BOTTOM:
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

