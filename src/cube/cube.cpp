#include "stdafx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <float.h>

#include "cube.h"
#include "vec2f.h"



#define ALMOST_ZERO   1e-6f




WCube::WCube(CWnd* pWnd)
: m_pWnd(pWnd)
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

            m_pPieces[x][y][z] = new WCubePiece(BYTEVEC(x-1, y-1, z-1));
         }
      }
   }

}


BOOL WCube::rotate(GLdouble* mxProjection, GLdouble* mxModel, GLint* nViewPort, 
                   CPoint& wndSize, CPoint& ptMouseWnd, CPoint& ptLastMouseWnd)
{
   CPoint ptMouse(ptMouseWnd.x, wndSize.y-ptMouseWnd.y);
   CPoint ptLastMouse(ptLastMouseWnd.x, wndSize.y-ptLastMouseWnd.y);
 
   PT3D vCubeCorner[8];

   // use glm::project instead of gluProject
   gluProject( 1.5, 1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[0].x, &vCubeCorner[0].y, &vCubeCorner[0].z);
   gluProject( 1.5,-1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[1].x, &vCubeCorner[1].y, &vCubeCorner[1].z);
   gluProject(-1.5,-1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[2].x, &vCubeCorner[2].y, &vCubeCorner[2].z);
   gluProject(-1.5, 1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[3].x, &vCubeCorner[3].y, &vCubeCorner[3].z);


   gluProject( 1.5, 1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[4].x, &vCubeCorner[4].y, &vCubeCorner[4].z);
   gluProject( 1.5,-1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[5].x, &vCubeCorner[5].y, &vCubeCorner[5].z);
   gluProject(-1.5,-1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[6].x, &vCubeCorner[6].y, &vCubeCorner[6].z);
   gluProject(-1.5, 1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[7].x, &vCubeCorner[7].y, &vCubeCorner[7].z);




   //Find the min/max X and Y to do a rough bounding box test.
   float xMin=FLT_MAX, yMin=FLT_MAX;
   float xMax=FLT_MIN, yMax=FLT_MIN;
   for (int i=0; i<8; i++) {
      xMin = min(xMin, (float)vCubeCorner[i].x);
      yMin = min(yMin, (float)vCubeCorner[i].y);
      xMax = max(xMax, (float)vCubeCorner[i].x);
      yMax = max(yMax, (float)vCubeCorner[i].y);
   }


   //Check if point was outside rough test and return if it was.
   if (!(xMin <= ptLastMouse.x && ptLastMouse.x <= xMax
         && yMin <= ptLastMouse.y && ptLastMouse.y <= yMax))
   {
      return false;        //Failed rough bounding box test
   }



   PT3D vCorner[6][4] = {
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
      if (poly4InsideTest(vCorner[i], ptLastMouse)) {
         fTmp = min(min(min(vCorner[i][0].z, vCorner[i][1].z), vCorner[i][2].z), vCorner[i][3].z);
         if (fTmp < fMinZ) {
            nSide = (SIDE)i;
            fMinZ = fTmp;
         }
      }
   }

   if (nSide==-1) 
      return FALSE;        //Missed all the sides.





   Vec2f vX(
      (float)(vCorner[nSide][1].x - vCorner[nSide][0].x),
      (float)(vCorner[nSide][1].y - vCorner[nSide][0].y)
   );

   Vec2f vY(
      (float)(vCorner[nSide][2].x - vCorner[nSide][1].x),
      (float)(vCorner[nSide][2].y - vCorner[nSide][1].y)
   );

   Vec2f vMouse(
      (float)(ptMouse.x - ptLastMouse.x), 
      (float)(ptMouse.y - ptLastMouse.y)
   );


   //angle between the mouse vector and the 
   //X/Y vector for this cube side.
   vX.normalize();
   vY.normalize();
   vMouse.normalize();

   float xDiff = Vec2f::getAngle(vX, vMouse);
   float yDiff = Vec2f::getAngle(vY, vMouse);
   float minDiff = (float)min(min(min(fabs(xDiff), fabs(yDiff)), fabs(xDiff-180)), fabs(yDiff-180));


   INT8 nSection;

   minDiff += ALMOST_ZERO;       

   if (fabs(xDiff) <= minDiff) {
      nSection = getYsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_BACK:     rotateYSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_LEFT:     rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_RIGHT:    rotateYSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_TOP:      rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
         case SD_BOTTOM:   rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
      }
   }
   else if (fabs(xDiff-180) <= minDiff) {
      nSection = getYsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateYSection(nSection, FALSE, TRUE, TRUE);    break;   
         case SD_BACK:     rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_LEFT:     rotateYSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_RIGHT:    rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_TOP:      rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
         case SD_BOTTOM:   rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
      }
   }
   else if (fabs(yDiff) <= minDiff) {
      nSection = getXsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_BACK:     rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
         case SD_LEFT:     rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
         case SD_RIGHT:    rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
         case SD_TOP:      rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
         case SD_BOTTOM:   rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
      }
   }
   else if (fabs(yDiff-180) <= minDiff) {
      nSection = getXsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
         case SD_BACK:     rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_LEFT:     rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
         case SD_RIGHT:    rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
         case SD_TOP:      rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_BOTTOM:   rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
      }
   }
   else {
      ASSERT(FALSE);
      return FALSE;
   }

   return TRUE;      
}




void WCube::animateRotation(WCubePiece* piece[], int ctPieces, Vec3f v, float fAngle)
{
   int x;

   ASSERT(m_pWnd);

   for (UINT i=0; i<35; i++) {
      float fRotAngle = fAngle * i/35.0f;

      for (int x=0; x<ctPieces; x++) 
         piece[x]->setRotation(fRotAngle, v);
      
      m_pWnd->Invalidate();
      m_pWnd->UpdateWindow();
   }

   for (x=0; x<ctPieces; x++) 
      piece[x]->clrRotation();
}





void WCube::rotateXSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord)
{
   WCubePiece* pTmp;
   int i;

   ASSERT(-1 <= nSection && nSection <= 1);

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
      animateRotation(pieces, ELEMENTS_OF(pieces), Vec3f(1,0,0), fAngle);

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

void WCube::rotateYSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord)
{
   WCubePiece* pTmp;
   int i;

   ASSERT(-1 <= nSection && nSection <= 1);

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
      animateRotation(pieces, ELEMENTS_OF(pieces), Vec3f(0,1,0), fAngle);


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

void WCube::rotateZSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord)
{
   WCubePiece* pTmp;
   int i;

   ASSERT(-1 <= nSection && nSection <= 1);

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
      animateRotation(pieces, ELEMENTS_OF(pieces), Vec3f(0,0,1), fAngle);


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