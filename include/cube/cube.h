#ifndef CUBE_H
#define CUBE_H


#include "util.h"
#include "piece.h"





class WCube
{
protected:
   WCubePiece* m_pPieces[3][3][3];
   CWnd*       m_pWnd;                    //Ptr to window that's displaying the cube


public:
   WCube(CWnd* pWnd);
   virtual ~WCube();

   void init(void);
   void draw(void);
   void reset(void);

   //Get piece based on cube coordinates
   WCubePiece*& getPiece(INT8 x, INT8 y, INT8 z)    {return m_pPieces[x+1][y+1][z+1];};

   //Translate a mouse movement to a cube rotation.
   // glm::mat4, glm::vec3, glm::vec4, (x, y) | glm::vec2
   BOOL rotate(GLdouble* mxProjection, GLdouble* mxModel, GLint* nViewport, 
               CPoint& wndSize, CPoint& ptMouseWnd, CPoint& ptLastMouseWnd);
   void rotateXSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord);
   void rotateYSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord);
   void rotateZSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord);
protected:
   void animateRotation(WCubePiece* piece[], int ctPieces, Vec3f v, float fAngle);
};




#endif //CUBE_H
