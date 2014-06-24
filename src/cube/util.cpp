#include <math.h>

#include "cube/util.h"


#define ALMOST_ZERO 1e-6     // error tolerance

bool poly4InsideTest(glm::vec2 *ptCorners, float ptX, float ptY)
{
    int Result1, Result2;

    Result1 = lineTest(ptCorners[0], ptCorners[1], ptX, ptY);
    Result2 = lineTest(ptCorners[2], ptCorners[3], ptX, ptY);

    if (((Result1 > 0) && (Result2 > 0))
        || ((Result1 < 0) && (Result2 < 0)))
    {
        return false;
    }


    Result1 = lineTest(ptCorners[0], ptCorners[3], ptX, ptY);
    Result2 = lineTest(ptCorners[1], ptCorners[2], ptX, ptY);

    if (((Result1 > 0) && (Result2 > 0))
        || ((Result1 < 0) && (Result2 < 0)))
    {
        return false;
    }


    return true;
}

bool poly4InsideTest(glm::vec3 *pt3Corners, float ptX, float ptY)
{
    glm::vec2 pt2Corners[4];

    for (int i=0; i<4; i++) {
        pt2Corners[i].x = pt3Corners[i].x;
        pt2Corners[i].y = pt3Corners[i].y;
    }

    return poly4InsideTest(pt2Corners, ptX, ptY);
}

int lineTest(const glm::vec2& ptLineStart, const glm::vec2& ptLineEnd, float ptX, float ptY)
{
    double dx = ptLineEnd.x - ptLineStart.x;
    double dy = ptLineEnd.y - ptLineStart.y;


    if (fabs(dx)>fabs(dy)) {
        double yOnline = dy/dx * (ptX - ptLineStart.x) + ptLineStart.y;
        return ptY >  yOnline
            ? 1
            : (ptY < yOnline ? -1 : 0);
    }
    else {
        double xOnline = dx/dy * (ptY - ptLineStart.y) + ptLineStart.x;
        return ptX >  xOnline
            ? 1
            : (ptX < xOnline ? -1 : 0);
    }
}


int getYsection(glm::vec2 *ptCorners, float ptX, float ptY)
{
    int rc0, rc1, rc2;
    glm::vec2 ptLineStart, ptLineEnd;
    double dx, dy;

    dx = (ptCorners[2].x - ptCorners[1].x)/3;
    dy = (ptCorners[2].y - ptCorners[1].y)/3;



    ptLineStart.x = ptCorners[0].x;
    ptLineStart.y = ptCorners[0].y;
    ptLineEnd.x   = ptCorners[1].x;
    ptLineEnd.y   = ptCorners[1].y;
    rc0 = lineTest(ptLineStart, ptLineEnd, ptX, ptY);

    ptLineStart.x += dx;
    ptLineStart.y += dy;
    ptLineEnd.x   += dx;
    ptLineEnd.y   += dy;
    rc1 = lineTest(ptLineStart, ptLineEnd, ptX, ptY);

    ptLineStart.x += dx;
    ptLineStart.y += dy;
    ptLineEnd.x   += dx;
    ptLineEnd.y   += dy;
    rc2 = lineTest(ptLineStart, ptLineEnd, ptX, ptY);

    if (((rc1 >= 0) && (rc2 <= 0) )
        || ((rc1 <= 0) && (rc2 >= 0)))
    {
        return 0;
    }


    if (rc0 == 0)
        return -1;


    if (((rc1 > 0) && (rc0 > 0))
        || ((rc1 < 0) && (rc0 < 0)))
    {
        return 1;
    }


    return -1;
}

int getYsection(glm::vec3 *pt3Corners, float ptX, float ptY)
{
    glm::vec2 pt2Corners[4];

    for (int i=0; i<4; i++) {
        pt2Corners[i].x = pt3Corners[i].x;
        pt2Corners[i].y = pt3Corners[i].y;
    }


    return getYsection(pt2Corners, ptX, ptY);
}


int getXsection(glm::vec2 *ptCorners, float ptX, float ptY)
{
    int rc0, rc1, rc2;
    glm::vec2 ptLineStart, ptLineEnd;
    double dx, dy;

    dx = (ptCorners[1].x - ptCorners[0].x)/3;
    dy = (ptCorners[1].y - ptCorners[0].y)/3;

    ptLineStart.x = ptCorners[0].x;
    ptLineStart.y = ptCorners[0].y;
    ptLineEnd.x   = ptCorners[3].x;
    ptLineEnd.y   = ptCorners[3].y;
    rc0 = lineTest(ptLineStart, ptLineEnd, ptX, ptY);

    ptLineStart.x += dx;
    ptLineStart.y += dy;
    ptLineEnd.x   += dx;
    ptLineEnd.y   += dy;
    rc1 = lineTest(ptLineStart, ptLineEnd, ptX, ptY);

    ptLineStart.x += dx;
    ptLineStart.y += dy;
    ptLineEnd.x   += dx;
    ptLineEnd.y   += dy;
    rc2 = lineTest(ptLineStart, ptLineEnd, ptX, ptY);

    if (((rc1 >= 0) && (rc2 <= 0))
        || ((rc1 <= 0) && (rc2 >= 0)))
    {
        return 0;
    }


    if (rc0 == 0)
        return -1;


    if (((rc1 > 0) && (rc0 > 0))
        || ((rc1 < 0) && (rc0 < 0)))
    {
        return 1;
    }


    return -1;
}


int getXsection(glm::vec3 *pt3Corners, float ptX, float ptY)
{
    glm::vec2 pt2Corners[4];

    for (int i=0; i<4; i++) {
        pt2Corners[i].x = pt3Corners[i].y;
        pt2Corners[i].x = pt3Corners[i].y;
    }

    return getXsection(pt2Corners, ptX, ptY);
}

