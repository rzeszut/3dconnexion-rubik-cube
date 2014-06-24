#include "stdafx.h"
#include <math.h>

#include "util.h"


#define ALMOST_ZERO 1e-6     // error tolerance

BOOL poly4InsideTest(const PT2D* ptCorners, const CPoint& pt)
{
   int nResult1, nResult2;

   nResult1 = lineTest(ptCorners[0], ptCorners[1], pt);
   nResult2 = lineTest(ptCorners[2], ptCorners[3], pt);

   if (nResult1 > 0 && nResult2 > 0 
       || nResult1 < 0 && nResult2 < 0)
   {
      return FALSE;
   }


   nResult1 = lineTest(ptCorners[0], ptCorners[3], pt);
   nResult2 = lineTest(ptCorners[1], ptCorners[2], pt);

   if (nResult1 > 0 && nResult2 > 0 
       || nResult1 < 0 && nResult2 < 0)
   {
      return FALSE;
   }


   return TRUE;
}

BOOL poly4InsideTest(PT3D* pt3Corners, const CPoint& pt)
{
   PT2D pt2Corners[4];

   for (int i=0; i<4; i++) 
      pt2Corners[i] = pt3Corners[i];

   return poly4InsideTest(pt2Corners, pt);
}

int lineTest(const PT2D& ptLineStart, const PT2D& ptLineEnd, const CPoint& pt)
{
   double dx = ptLineEnd.x - ptLineStart.x;
   double dy = ptLineEnd.y - ptLineStart.y;


   if (fabs(dx)>fabs(dy)) {
      double yOnline = dy/dx * (pt.x - ptLineStart.x) + ptLineStart.y;
      return pt.y >  yOnline 
             ? 1
             : (pt.y < yOnline ? -1 : 0);
   }
   else {
      double xOnline = dx/dy * (pt.y - ptLineStart.y) + ptLineStart.x;
      return pt.x >  xOnline 
             ? 1
             : (pt.x < xOnline ? -1 : 0);
   }
}


INT8 getYsection(PT2D* ptCorners, const CPoint& pt)
{
   int rc0, rc1, rc2;
   PT2D ptLineStart, ptLineEnd;
   double dx, dy;

   dx = (ptCorners[2].x - ptCorners[1].x)/3;
   dy = (ptCorners[2].y - ptCorners[1].y)/3;



   ptLineStart.x = ptCorners[0].x;
   ptLineStart.y = ptCorners[0].y;
   ptLineEnd.x   = ptCorners[1].x;
   ptLineEnd.y   = ptCorners[1].y;
   rc0 = lineTest(ptLineStart, ptLineEnd, pt);

   ptLineStart.x += dx;
   ptLineStart.y += dy;
   ptLineEnd.x   += dx;
   ptLineEnd.y   += dy;
   rc1 = lineTest(ptLineStart, ptLineEnd, pt);

   ptLineStart.x += dx;
   ptLineStart.y += dy;
   ptLineEnd.x   += dx;
   ptLineEnd.y   += dy;
   rc2 = lineTest(ptLineStart, ptLineEnd, pt);

   if (rc1 >= 0 && rc2 <= 0 
       || rc1 <= 0 && rc2 >= 0)
   {
      return 0;
   }


   if (rc0 == 0)
      return -1;


   if (rc1 > 0 && rc0 > 0 
       || rc1 < 0 && rc0 < 0)
   {
      return 1;
   }

       
   return -1;
}

INT8 getYsection(PT3D* pt3Corners, const CPoint& pt)
{
   PT2D pt2Corners[4];

   for (int i=0; i<4; i++) 
      pt2Corners[i] = pt3Corners[i];

   return getYsection(pt2Corners, pt);
}


INT8 getXsection(PT2D* ptCorners, const CPoint& pt)
{
   int rc0, rc1, rc2;
   PT2D ptLineStart, ptLineEnd;
   double dx, dy;

   dx = (ptCorners[1].x - ptCorners[0].x)/3;
   dy = (ptCorners[1].y - ptCorners[0].y)/3;

   ptLineStart.x = ptCorners[0].x;
   ptLineStart.y = ptCorners[0].y;
   ptLineEnd.x   = ptCorners[3].x;
   ptLineEnd.y   = ptCorners[3].y;
   rc0 = lineTest(ptLineStart, ptLineEnd, pt);
   
   ptLineStart.x += dx;
   ptLineStart.y += dy;
   ptLineEnd.x   += dx;
   ptLineEnd.y   += dy;
   rc1 = lineTest(ptLineStart, ptLineEnd, pt);

   ptLineStart.x += dx;
   ptLineStart.y += dy;
   ptLineEnd.x   += dx;
   ptLineEnd.y   += dy;
   rc2 = lineTest(ptLineStart, ptLineEnd, pt);

   if (rc1 >= 0 && rc2 <= 0 
       || rc1 <= 0 && rc2 >= 0)
   {
      return 0;
   }


   if (rc0 == 0)
      return -1;


   if (rc1 > 0 && rc0 > 0 
       || rc1 < 0 && rc0 < 0)
   {
      return 1;
   }

       
   return -1;
}


INT8 getXsection(PT3D* pt3Corners, const CPoint& pt)
{
   PT2D pt2Corners[4];

   for (int i=0; i<4; i++) 
      pt2Corners[i] = pt3Corners[i];

   return getXsection(pt2Corners, pt);
}

