#include <cmath>
#include <cfloat>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "cube/cube.h"



#define ALMOST_ZERO   1e-6f

WCube::WCube()
{
    memset(m_pPieces, 0, sizeof(WCubePiece*)*3*3*3);
}

WCube::~WCube()
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

void WCube::init(void)
{
    reset();
}

void WCube::draw(void)
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

void WCube::reset(void)
{
    for (int x=0; x<3; x++) {
        for (int y=0; y<3; y++) {
            for (int z=0; z<3; z++) {
                if (m_pPieces[x][y][z])
                    delete m_pPieces[x][y][z];

                m_pPieces[x][y][z] = new WCubePiece(glm::ivec3(x-1, y-1, z-1));
            }
        }
    }

}

bool WCube::rotate(glm::mat4 mxProjection, glm::mat4 mxModelView, glm::vec4 nViewport,
                   glm::vec2 wndSize, glm::vec2 ptMouseWnd, glm::vec2 ptLastMouseWnd)
{
    static bool animate = false;

    glm::vec2 ptMouse(ptMouseWnd.x, wndSize.y-ptMouseWnd.y);
    glm::vec2 ptLastMouse(ptLastMouseWnd.x, wndSize.y-ptLastMouseWnd.y);

    glm::vec3 vCubeCorner[8];

    // use glm::project instead of gluProject
    vCubeCorner[0] = glm::project(glm::vec3(1.5, 1.5, 1.5), mxModelView, mxProjection, nViewport);
    vCubeCorner[1] = glm::project(glm::vec3(1.5,-1.5, 1.5), mxModelView, mxProjection, nViewport);
    vCubeCorner[2] = glm::project(glm::vec3(-1.5,-1.5, 1.5), mxModelView, mxProjection, nViewport);
    vCubeCorner[3] = glm::project(glm::vec3(-1.5, 1.5, 1.5), mxModelView, mxProjection, nViewport);

    vCubeCorner[4] = glm::project(glm::vec3(1.5, 1.5,-1.5), mxModelView, mxProjection, nViewport);
    vCubeCorner[5] = glm::project(glm::vec3(1.5,-1.5,-1.5), mxModelView, mxProjection, nViewport);
    vCubeCorner[6] = glm::project(glm::vec3(-1.5,-1.5,-1.5), mxModelView, mxProjection, nViewport);
    vCubeCorner[7] = glm::project(glm::vec3(-1.5, 1.5,-1.5), mxModelView, mxProjection, nViewport);


    //Find the min/max X and Y to do a rough bounding box test.
    float xMin=FLT_MAX, yMin=FLT_MAX;
    float xMax=FLT_MIN, yMax=FLT_MIN;
    for (int i=0; i<8; i++) {
        xMin = fmin(xMin, (float)vCubeCorner[i].x);
        yMin = fmin(yMin, (float)vCubeCorner[i].y);
        xMax = fmax(xMax, (float)vCubeCorner[i].x);
        yMax = fmax(yMax, (float)vCubeCorner[i].y);
    }

    //Check if point was outside rough test and return if it was.
    if (!(xMin <= ptLastMouse.x && ptLastMouse.x <= xMax
          && yMin <= ptLastMouse.y && ptLastMouse.y <= yMax))
    {
        return false;        //Failed rough bounding box test
    }

    glm::vec3 vCorner[6][4] = {
        {vCubeCorner[5], vCubeCorner[1], vCubeCorner[0], vCubeCorner[4]},    //Right
        {vCubeCorner[6], vCubeCorner[2], vCubeCorner[3], vCubeCorner[7]},    //Left
        {vCubeCorner[7], vCubeCorner[4], vCubeCorner[0], vCubeCorner[3]},    //Top
        {vCubeCorner[6], vCubeCorner[5], vCubeCorner[1], vCubeCorner[2]},    //Bottom
        {vCubeCorner[2], vCubeCorner[1], vCubeCorner[0], vCubeCorner[3]},    //Front
        {vCubeCorner[6], vCubeCorner[5], vCubeCorner[4], vCubeCorner[7]},    //Back
    };

    double fMinZ = DBL_MAX;
    double fTmp;
    SIDE nSide = (SIDE)-1;


    for (int i=0; i<6; i++) {
        if (poly4InsideTest(vCorner[i], ptLastMouse.x, ptLastMouse.y)) {
            fTmp = fmin(fmin(fmin(vCorner[i][0].z, vCorner[i][1].z), vCorner[i][2].z), vCorner[i][3].z);
            if (fTmp < fMinZ) {
                nSide = (SIDE)i;
                fMinZ = fTmp;
            }
        }
    }

    if ((int) nSide == -1)
        return false;        //Missed all the sides.

    glm::vec2 vX(
        (float)(vCorner[nSide][1].x - vCorner[nSide][0].x),
        (float)(vCorner[nSide][1].y - vCorner[nSide][0].y)
        );

    glm::vec2 vY(
        (float)(vCorner[nSide][2].x - vCorner[nSide][1].x),
        (float)(vCorner[nSide][2].y - vCorner[nSide][1].y)
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

    int nSection;

    minDiff += ALMOST_ZERO;

    if (fabs(xDiff) <= minDiff) {
        nSection = getYsection(vCorner[nSide], ptLastMouse.x ,ptLastMouse.y);
        switch (nSide) {
        case SD_FRONT:    rotateYSection(nSection, true, animate, true);     break;
        case SD_BACK:     rotateYSection(nSection, false, animate, true);    break;
        case SD_LEFT:     rotateYSection(nSection, true, animate, true);     break;
        case SD_RIGHT:    rotateYSection(nSection, false, animate, true);    break;
        case SD_TOP:      rotateZSection(nSection, false, animate, true);     break;
        case SD_BOTTOM:   rotateZSection(nSection, true, animate, true);    break;
        }
    }
    else if (fabs(xDiff-180) <= minDiff) {
        nSection = getYsection(vCorner[nSide], ptLastMouse.x, ptLastMouse.y);
        switch (nSide) {
        case SD_FRONT:    rotateYSection(nSection, false, animate, true);    break;
        case SD_BACK:     rotateYSection(nSection, true, animate, true);     break;
        case SD_LEFT:     rotateYSection(nSection, false, animate, true);    break;
        case SD_RIGHT:    rotateYSection(nSection, true, animate, true);     break;
        case SD_TOP:      rotateZSection(nSection, true, animate, true);    break;
        case SD_BOTTOM:   rotateZSection(nSection, false, animate, true);     break;
        }
    }
    else if (fabs(yDiff) <= minDiff) {
        nSection = getXsection(vCorner[nSide], ptLastMouse.x, ptLastMouse.y);
        switch (nSide) {
        case SD_FRONT:    rotateXSection(nSection, false, animate, true);    break;
        case SD_BACK:     rotateXSection(nSection, true,  animate, true);    break;
        case SD_LEFT:     rotateZSection(nSection, false, animate, true);     break;
        case SD_RIGHT:    rotateZSection(nSection, true, animate, true);    break;
        case SD_TOP:      rotateXSection(nSection, true,  animate, true);    break;
        case SD_BOTTOM:   rotateXSection(nSection, false, animate, true);    break;
        }
    }
    else if (fabs(yDiff-180) <= minDiff) {
        nSection = getXsection(vCorner[nSide], ptLastMouse.x, ptLastMouse.y);
        switch (nSide) {
        case SD_FRONT:    rotateXSection(nSection, true,  animate, true);    break;
        case SD_BACK:     rotateXSection(nSection, false, animate, true);    break;
        case SD_LEFT:     rotateZSection(nSection, true, animate, true);    break;
        case SD_RIGHT:    rotateZSection(nSection, false, animate, true);     break;
        case SD_TOP:      rotateXSection(nSection, false, animate, true);    break;
        case SD_BOTTOM:   rotateXSection(nSection, true,  animate, true);    break;
        }
    }
    else {
        return false;
    }

    return true;
}

void WCube::animateRotation(WCubePiece* piece[], int ctPieces, glm::vec3 v, float fAngle)
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

void WCube::rotateXSection(int nSection, bool bCW, bool bAnimate, bool bRecord)
{
    WCubePiece* pTmp;
    int i;

    //ASSERT(-1 <= nSection && nSection <= 1);

    WCubePiece* pieces[] = {
        getPiece(nSection, 1, 1),
        getPiece(nSection,-1, 1),
        getPiece(nSection,-1,-1),
        getPiece(nSection, 1,-1),
        getPiece(nSection, 1, 0),
        getPiece(nSection, 0, 1),
        getPiece(nSection,-1, 0),
        getPiece(nSection, 0,-1),
        getPiece(nSection, 0, 0),
    };

    float fAngle = bCW ? 90.0f : -90.0f;
    if (bAnimate)
        animateRotation(pieces, ELEMENTS_OF(pieces), glm::vec3(1,0,0), fAngle);

    for (i=0; i<ELEMENTS_OF(pieces); i++)
        pieces[i]->rotateX(bCW);


    if (bCW) {
        pTmp = getPiece(nSection, 1, 1);
        getPiece(nSection, 1, 1) = getPiece(nSection, 1,-1);
        getPiece(nSection, 1,-1) = getPiece(nSection,-1,-1);
        getPiece(nSection,-1,-1) = getPiece(nSection,-1, 1);
        getPiece(nSection,-1, 1) = pTmp;

        pTmp = getPiece(nSection, 1, 0);
        getPiece(nSection, 1, 0) = getPiece(nSection, 0,-1);
        getPiece(nSection, 0,-1) = getPiece(nSection,-1, 0);
        getPiece(nSection,-1, 0) = getPiece(nSection, 0, 1);
        getPiece(nSection, 0, 1) = pTmp;
    }
    else {
        pTmp = getPiece(nSection, 1, 1);
        getPiece(nSection, 1, 1) = getPiece(nSection,-1, 1);
        getPiece(nSection,-1, 1) = getPiece(nSection,-1,-1);
        getPiece(nSection,-1,-1) = getPiece(nSection, 1,-1);
        getPiece(nSection, 1,-1) = pTmp;

        pTmp = getPiece(nSection, 1, 0);
        getPiece(nSection, 1, 0) = getPiece(nSection, 0, 1);
        getPiece(nSection, 0, 1) = getPiece(nSection,-1, 0);
        getPiece(nSection,-1, 0) = getPiece(nSection, 0,-1);
        getPiece(nSection, 0,-1) = pTmp;

    }

}

void WCube::rotateYSection(int nSection, bool bCW, bool bAnimate, bool bRecord)
{
    WCubePiece* pTmp;
    int i;

    //ASSERT(-1 <= nSection && nSection <= 1);

    WCubePiece* pieces[] = {
        getPiece( 1, nSection, 1),
        getPiece(-1, nSection, 1),
        getPiece(-1, nSection,-1),
        getPiece( 1, nSection,-1),
        getPiece( 1, nSection, 0),
        getPiece( 0, nSection, 1),
        getPiece(-1, nSection, 0),
        getPiece( 0, nSection,-1),
        getPiece( 0, nSection, 0)
    };

    float fAngle = bCW ? 90.0f : -90.0f;
    if (bAnimate)
        animateRotation(pieces, ELEMENTS_OF(pieces), glm::vec3(0,1,0), fAngle);


    for (i=0; i<ELEMENTS_OF(pieces); i++)
        pieces[i]->rotateY(bCW);


    if (bCW) {
        pTmp = getPiece(1, nSection, 1);
        getPiece( 1, nSection, 1) = getPiece(-1, nSection, 1);
        getPiece(-1, nSection, 1) = getPiece(-1, nSection,-1);
        getPiece(-1, nSection,-1) = getPiece( 1, nSection,-1);
        getPiece( 1, nSection,-1) = pTmp;

        pTmp = getPiece(1, nSection, 0);
        getPiece( 1, nSection, 0) = getPiece( 0, nSection, 1);
        getPiece( 0, nSection, 1) = getPiece(-1, nSection, 0);
        getPiece(-1, nSection, 0) = getPiece( 0, nSection,-1);
        getPiece( 0, nSection,-1) = pTmp;
    }
    else {
        pTmp = getPiece(1, nSection, 1);
        getPiece( 1, nSection, 1) = getPiece( 1, nSection,-1);
        getPiece( 1, nSection,-1) = getPiece(-1, nSection,-1);
        getPiece(-1, nSection,-1) = getPiece(-1, nSection, 1);
        getPiece(-1, nSection, 1) = pTmp;

        pTmp = getPiece(1, nSection, 0);
        getPiece( 1, nSection, 0) = getPiece( 0, nSection,-1);
        getPiece( 0, nSection,-1) = getPiece(-1, nSection, 0);
        getPiece(-1, nSection, 0) = getPiece( 0, nSection, 1);
        getPiece( 0, nSection, 1) = pTmp;
    }

}

void WCube::rotateZSection(int nSection, bool bCW, bool bAnimate, bool bRecord)
{
    WCubePiece* pTmp;
    int i;

    //ASSERT(-1 <= nSection && nSection <= 1);

    WCubePiece* pieces[] = {
        getPiece( 1, 1, nSection),
        getPiece(-1, 1, nSection),
        getPiece(-1,-1, nSection),
        getPiece( 1,-1, nSection),
        getPiece( 1, 0, nSection),
        getPiece( 0, 1, nSection),
        getPiece(-1, 0, nSection),
        getPiece( 0,-1, nSection),
        getPiece( 0, 0, nSection)
    };

    float fAngle = bCW ? 90.0f : -90.0f;
    if (bAnimate)
        animateRotation(pieces, ELEMENTS_OF(pieces), glm::vec3(0,0,1), fAngle);


    for (i=0; i<ELEMENTS_OF(pieces); i++)
        pieces[i]->rotateZ(bCW);


    if (bCW) {
        pTmp = getPiece( 1, 1, nSection);
        getPiece( 1, 1, nSection) = getPiece( 1,-1, nSection);
        getPiece( 1,-1, nSection) = getPiece(-1,-1, nSection);
        getPiece(-1,-1, nSection) = getPiece(-1, 1, nSection);
        getPiece(-1, 1, nSection) = pTmp;

        pTmp = getPiece( 1, 0, nSection);
        getPiece( 1, 0, nSection) = getPiece( 0,-1, nSection);
        getPiece( 0,-1, nSection) = getPiece(-1, 0, nSection);
        getPiece(-1, 0, nSection) = getPiece( 0, 1, nSection);
        getPiece( 0, 1, nSection) = pTmp;
    }
    else {
        pTmp = getPiece( 1, 1, nSection);
        getPiece( 1, 1, nSection) = getPiece(-1, 1, nSection);
        getPiece(-1, 1, nSection) = getPiece(-1,-1, nSection);
        getPiece(-1,-1, nSection) = getPiece( 1,-1, nSection);
        getPiece( 1,-1, nSection) = pTmp;

        pTmp = getPiece( 1, 0, nSection);
        getPiece( 1, 0, nSection) = getPiece( 0, 1, nSection);
        getPiece( 0, 1, nSection) = getPiece(-1, 0, nSection);
        getPiece(-1, 0, nSection) = getPiece( 0,-1, nSection);
        getPiece( 0,-1, nSection) = pTmp;
    }

}
