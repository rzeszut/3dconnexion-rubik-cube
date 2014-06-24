#include <cmath>
#include <cfloat>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "cube/cube.h"

#define ALMOST_ZERO   1e-6f

Cube::Cube()
{
    memset(m_pPieces, 0, sizeof(CubePiece*)*3*3*3);
}

Cube::~Cube()
{
    for (int x=0; x<3; x++) {
        for (int y=0; y<3; y++) {
            for (int z=0; z<3; z++) {
                if (m_pPieces[x][y][z])
                    delete m_pPieces[x][y][z];
            }
        }
    }
}

void Cube::init(void)
{
    reset();
}

void Cube::draw(void)
{
    int x,y,z;

    for (x=-1; x<=1; x++) {
        for (y=-1; y<=1; y++) {
            for (z=-1; z<=1; z++) {
                getPiece(x,y,z)->draw(x,y,z);
            }
        }
    }
}

void Cube::reset(void)
{
    for (int x=0; x<3; x++) {
        for (int y=0; y<3; y++) {
            for (int z=0; z<3; z++) {
                if (m_pPieces[x][y][z])
                    delete m_pPieces[x][y][z];

                m_pPieces[x][y][z] = new CubePiece(glm::ivec3(x-1, y-1, z-1));
            }
        }
    }

}

bool Cube::rotate(glm::mat4 Projection, glm::mat4 ModelView, glm::vec4 Viewport,
                   glm::vec2 wndSize, glm::vec2 ptMouseWnd, glm::vec2 ptLastMouseWnd)
{
    static bool animate = false;

    glm::vec2 ptMouse(ptMouseWnd.x, wndSize.y-ptMouseWnd.y);
    glm::vec2 ptLastMouse(ptLastMouseWnd.x, wndSize.y-ptLastMouseWnd.y);

    glm::vec3 CubeCorner[8];

    // use glm::project instead of gluProject
    CubeCorner[0] = glm::project(glm::vec3(1.5, 1.5, 1.5), ModelView, Projection, Viewport);
    CubeCorner[1] = glm::project(glm::vec3(1.5,-1.5, 1.5), ModelView, Projection, Viewport);
    CubeCorner[2] = glm::project(glm::vec3(-1.5,-1.5, 1.5), ModelView, Projection, Viewport);
    CubeCorner[3] = glm::project(glm::vec3(-1.5, 1.5, 1.5), ModelView, Projection, Viewport);

    CubeCorner[4] = glm::project(glm::vec3(1.5, 1.5,-1.5), ModelView, Projection, Viewport);
    CubeCorner[5] = glm::project(glm::vec3(1.5,-1.5,-1.5), ModelView, Projection, Viewport);
    CubeCorner[6] = glm::project(glm::vec3(-1.5,-1.5,-1.5), ModelView, Projection, Viewport);
    CubeCorner[7] = glm::project(glm::vec3(-1.5, 1.5,-1.5), ModelView, Projection, Viewport);


    //Find the min/max X and Y to do a rough bounding box test.
    float xMin=FLT_MAX, yMin=FLT_MAX;
    float xMax=FLT_MIN, yMax=FLT_MIN;
    for (int i=0; i<8; i++) {
        xMin = fmin(xMin, (float)CubeCorner[i].x);
        yMin = fmin(yMin, (float)CubeCorner[i].y);
        xMax = fmax(xMax, (float)CubeCorner[i].x);
        yMax = fmax(yMax, (float)CubeCorner[i].y);
    }

    //Check if point was outside rough test and return if it was.
    if (!(xMin <= ptLastMouse.x && ptLastMouse.x <= xMax
          && yMin <= ptLastMouse.y && ptLastMouse.y <= yMax))
    {
        return false;        //Failed rough bounding box test
    }

    glm::vec3 Corner[6][4] = {
        {CubeCorner[5], CubeCorner[1], CubeCorner[0], CubeCorner[4]},    //Right
        {CubeCorner[6], CubeCorner[2], CubeCorner[3], CubeCorner[7]},    //Left
        {CubeCorner[7], CubeCorner[4], CubeCorner[0], CubeCorner[3]},    //Top
        {CubeCorner[6], CubeCorner[5], CubeCorner[1], CubeCorner[2]},    //Bottom
        {CubeCorner[2], CubeCorner[1], CubeCorner[0], CubeCorner[3]},    //Front
        {CubeCorner[6], CubeCorner[5], CubeCorner[4], CubeCorner[7]},    //Back
    };

    double fMinZ = DBL_MAX;
    double fTmp;
    SIDE Side = (SIDE)-1;


    for (int i=0; i<6; i++) {
        if (poly4InsideTest(Corner[i], ptLastMouse.x, ptLastMouse.y)) {
            fTmp = fmin(fmin(fmin(Corner[i][0].z, Corner[i][1].z), Corner[i][2].z), Corner[i][3].z);
            if (fTmp < fMinZ) {
                Side = (SIDE)i;
                fMinZ = fTmp;
            }
        }
    }

    if ((int) Side == -1)
        return false;        //Missed all the sides.

    glm::vec2 vX(
        (float)(Corner[Side][1].x - Corner[Side][0].x),
        (float)(Corner[Side][1].y - Corner[Side][0].y)
        );

    glm::vec2 vY(
        (float)(Corner[Side][2].x - Corner[Side][1].x),
        (float)(Corner[Side][2].y - Corner[Side][1].y)
        );

    glm::vec2 vMouse(
        (float)(ptMouse.x - ptLastMouse.x),
        (float)(ptMouse.y - ptLastMouse.y)
        );

    //angle between the mouse vector and the
    //X/Y vector for this cube side.
    vX = glm::normalize(vX);
    vY = glm::normalize(vY);
    vMouse = glm::normalize(vMouse);

    float xDiff = glm::angle(vX, vMouse);
    float yDiff = glm::angle(vY, vMouse);
    float minDiff = (float)fmin(fmin(fmin(fabs(xDiff), fabs(yDiff)), fabs(xDiff-180)), fabs(yDiff-180));

    int Section;

    minDiff += ALMOST_ZERO;

    if (fabs(xDiff) <= minDiff) {
        Section = getYsection(Corner[Side], ptLastMouse.x ,ptLastMouse.y);
        switch (Side) {
        case SD_FRONT:    rotateYSection(Section, true, animate, true);     break;
        case SD_BACK:     rotateYSection(Section, false, animate, true);    break;
        case SD_LEFT:     rotateYSection(Section, true, animate, true);     break;
        case SD_RIGHT:    rotateYSection(Section, false, animate, true);    break;
        case SD_TOP:      rotateZSection(Section, false, animate, true);     break;
        case SD_BOTTOM:   rotateZSection(Section, true, animate, true);    break;
        }
    }
    else if (fabs(xDiff-180) <= minDiff) {
        Section = getYsection(Corner[Side], ptLastMouse.x, ptLastMouse.y);
        switch (Side) {
        case SD_FRONT:    rotateYSection(Section, false, animate, true);    break;
        case SD_BACK:     rotateYSection(Section, true, animate, true);     break;
        case SD_LEFT:     rotateYSection(Section, false, animate, true);    break;
        case SD_RIGHT:    rotateYSection(Section, true, animate, true);     break;
        case SD_TOP:      rotateZSection(Section, true, animate, true);    break;
        case SD_BOTTOM:   rotateZSection(Section, false, animate, true);     break;
        }
    }
    else if (fabs(yDiff) <= minDiff) {
        Section = getXsection(Corner[Side], ptLastMouse.x, ptLastMouse.y);
        switch (Side) {
        case SD_FRONT:    rotateXSection(Section, false, animate, true);    break;
        case SD_BACK:     rotateXSection(Section, true,  animate, true);    break;
        case SD_LEFT:     rotateZSection(Section, false, animate, true);     break;
        case SD_RIGHT:    rotateZSection(Section, true, animate, true);    break;
        case SD_TOP:      rotateXSection(Section, true,  animate, true);    break;
        case SD_BOTTOM:   rotateXSection(Section, false, animate, true);    break;
        }
    }
    else if (fabs(yDiff-180) <= minDiff) {
        Section = getXsection(Corner[Side], ptLastMouse.x, ptLastMouse.y);
        switch (Side) {
        case SD_FRONT:    rotateXSection(Section, true,  animate, true);    break;
        case SD_BACK:     rotateXSection(Section, false, animate, true);    break;
        case SD_LEFT:     rotateZSection(Section, true, animate, true);    break;
        case SD_RIGHT:    rotateZSection(Section, false, animate, true);     break;
        case SD_TOP:      rotateXSection(Section, false, animate, true);    break;
        case SD_BOTTOM:   rotateXSection(Section, true,  animate, true);    break;
        }
    }
    else {
        return false;
    }

    return true;
}

void Cube::animateRotation(CubePiece* piece[], int ctPieces, glm::vec3 v, float fAngle)
{
    int x;

    for (unsigned i=0; i<35; i++) {
        float fRotAngle = fAngle * i/35.0f;

        for (int x=0; x<ctPieces; x++)
            piece[x]->setRotation(fRotAngle, v);

        //m_pWnd->Invalidate();
        //m_pWnd->UpdateWindow();
    }

    for (x=0; x<ctPieces; x++)
        piece[x]->clrRotation();
}

void Cube::rotateXSection(int Section, bool bCW, bool bAnimate, bool bRecord)
{
    CubePiece* pTmp;
    int i;

    //ASSERT(-1 <= nSection && nSection <= 1);

    CubePiece* pieces[] = {
        getPiece(Section, 1, 1),
        getPiece(Section,-1, 1),
        getPiece(Section,-1,-1),
        getPiece(Section, 1,-1),
        getPiece(Section, 1, 0),
        getPiece(Section, 0, 1),
        getPiece(Section,-1, 0),
        getPiece(Section, 0,-1),
        getPiece(Section, 0, 0),
    };

    float fAngle = bCW ? 90.0f : -90.0f;
    if (bAnimate)
        animateRotation(pieces, ELEMENTS_OF(pieces), glm::vec3(1,0,0), fAngle);

    for (i=0; i<ELEMENTS_OF(pieces); i++)
        pieces[i]->rotateX(bCW);


    if (bCW) {
        pTmp = getPiece(Section, 1, 1);
        getPiece(Section, 1, 1) = getPiece(Section, 1,-1);
        getPiece(Section, 1,-1) = getPiece(Section,-1,-1);
        getPiece(Section,-1,-1) = getPiece(Section,-1, 1);
        getPiece(Section,-1, 1) = pTmp;

        pTmp = getPiece(Section, 1, 0);
        getPiece(Section, 1, 0) = getPiece(Section, 0,-1);
        getPiece(Section, 0,-1) = getPiece(Section,-1, 0);
        getPiece(Section,-1, 0) = getPiece(Section, 0, 1);
        getPiece(Section, 0, 1) = pTmp;
    }
    else {
        pTmp = getPiece(Section, 1, 1);
        getPiece(Section, 1, 1) = getPiece(Section,-1, 1);
        getPiece(Section,-1, 1) = getPiece(Section,-1,-1);
        getPiece(Section,-1,-1) = getPiece(Section, 1,-1);
        getPiece(Section, 1,-1) = pTmp;

        pTmp = getPiece(Section, 1, 0);
        getPiece(Section, 1, 0) = getPiece(Section, 0, 1);
        getPiece(Section, 0, 1) = getPiece(Section,-1, 0);
        getPiece(Section,-1, 0) = getPiece(Section, 0,-1);
        getPiece(Section, 0,-1) = pTmp;

    }

}

void Cube::rotateYSection(int Section, bool bCW, bool bAnimate, bool bRecord)
{
    CubePiece* pTmp;
    int i;

    //ASSERT(-1 <= nSection && nSection <= 1);

    CubePiece* pieces[] = {
        getPiece( 1, Section, 1),
        getPiece(-1, Section, 1),
        getPiece(-1, Section,-1),
        getPiece( 1, Section,-1),
        getPiece( 1, Section, 0),
        getPiece( 0, Section, 1),
        getPiece(-1, Section, 0),
        getPiece( 0, Section,-1),
        getPiece( 0, Section, 0)
    };

    float fAngle = bCW ? 90.0f : -90.0f;
    if (bAnimate)
        animateRotation(pieces, ELEMENTS_OF(pieces), glm::vec3(0,1,0), fAngle);


    for (i=0; i<ELEMENTS_OF(pieces); i++)
        pieces[i]->rotateY(bCW);


    if (bCW) {
        pTmp = getPiece(1, Section, 1);
        getPiece( 1, Section, 1) = getPiece(-1, Section, 1);
        getPiece(-1, Section, 1) = getPiece(-1, Section,-1);
        getPiece(-1, Section,-1) = getPiece( 1, Section,-1);
        getPiece( 1, Section,-1) = pTmp;

        pTmp = getPiece(1, Section, 0);
        getPiece( 1, Section, 0) = getPiece( 0, Section, 1);
        getPiece( 0, Section, 1) = getPiece(-1, Section, 0);
        getPiece(-1, Section, 0) = getPiece( 0, Section,-1);
        getPiece( 0, Section,-1) = pTmp;
    }
    else {
        pTmp = getPiece(1, Section, 1);
        getPiece( 1, Section, 1) = getPiece( 1, Section,-1);
        getPiece( 1, Section,-1) = getPiece(-1, Section,-1);
        getPiece(-1, Section,-1) = getPiece(-1, Section, 1);
        getPiece(-1, Section, 1) = pTmp;

        pTmp = getPiece(1, Section, 0);
        getPiece( 1, Section, 0) = getPiece( 0, Section,-1);
        getPiece( 0, Section,-1) = getPiece(-1, Section, 0);
        getPiece(-1, Section, 0) = getPiece( 0, Section, 1);
        getPiece( 0, Section, 1) = pTmp;
    }

}

void Cube::rotateZSection(int Section, bool bCW, bool bAnimate, bool bRecord)
{
    CubePiece* pTmp;
    int i;

    //ASSERT(-1 <= nSection && nSection <= 1);

    CubePiece* pieces[] = {
        getPiece( 1, 1, Section),
        getPiece(-1, 1, Section),
        getPiece(-1,-1, Section),
        getPiece( 1,-1, Section),
        getPiece( 1, 0, Section),
        getPiece( 0, 1, Section),
        getPiece(-1, 0, Section),
        getPiece( 0,-1, Section),
        getPiece( 0, 0, Section)
    };

    float fAngle = bCW ? 90.0f : -90.0f;
    if (bAnimate)
        animateRotation(pieces, ELEMENTS_OF(pieces), glm::vec3(0,0,1), fAngle);


    for (i=0; i<ELEMENTS_OF(pieces); i++)
        pieces[i]->rotateZ(bCW);


    if (bCW) {
        pTmp = getPiece( 1, 1, Section);
        getPiece( 1, 1, Section) = getPiece( 1,-1, Section);
        getPiece( 1,-1, Section) = getPiece(-1,-1, Section);
        getPiece(-1,-1, Section) = getPiece(-1, 1, Section);
        getPiece(-1, 1, Section) = pTmp;

        pTmp = getPiece( 1, 0, Section);
        getPiece( 1, 0, Section) = getPiece( 0,-1, Section);
        getPiece( 0,-1, Section) = getPiece(-1, 0, Section);
        getPiece(-1, 0, Section) = getPiece( 0, 1, Section);
        getPiece( 0, 1, Section) = pTmp;
    }
    else {
        pTmp = getPiece( 1, 1, Section);
        getPiece( 1, 1, Section) = getPiece(-1, 1, Section);
        getPiece(-1, 1, Section) = getPiece(-1,-1, Section);
        getPiece(-1,-1, Section) = getPiece( 1,-1, Section);
        getPiece( 1,-1, Section) = pTmp;

        pTmp = getPiece( 1, 0, Section);
        getPiece( 1, 0, Section) = getPiece( 0, 1, Section);
        getPiece( 0, 1, Section) = getPiece(-1, 0, Section);
        getPiece(-1, 0, Section) = getPiece( 0,-1, Section);
        getPiece( 0,-1, Section) = pTmp;
    }

}
