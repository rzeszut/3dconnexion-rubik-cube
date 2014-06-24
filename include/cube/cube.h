#ifndef CUBE_H
#define CUBE_H


#include "util.h"
#include "piece.h"
#include "glm/glm.hpp"

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
   WCubePiece*& getPiece(int x, int y, int z)    {return m_pPieces[x+1][y+1][z+1];};

   //Translate a mouse movement to a cube rotation.
   // glm::mat4, glm::vec3, glm::vec4, (x, y) | glm::vec2
   bool rotate(glm::mat4 mxProjection, glm::mat4 mxModel, glm::mat4 mxView, glm::vec4 nViewport, 
               glm::vec2 wndSize, glm::vec2 ptMouseWnd, glm::vec2 ptLastMouseWnd);
   void rotateXSection(int nSection, bool bCW, bool bAnimate, bool bRecord);
   void rotateYSection(int nSection, bool bCW, bool bAnimate, bool bRecord);
   void rotateZSection(int nSection, bool bCW, bool bAnimate, bool bRecord);
protected:
   void animateRotation(WCubePiece* piece[], int ctPieces, glm::vec3 v, float fAngle);
};

#endif //CUBE_H
