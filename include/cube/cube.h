#ifndef CUBE_H
#define CUBE_H


#include "util.h"
#include "piece.h"
#include "glm/glm.hpp"

class Cube
{
protected:
    CubePiece* m_pPieces[3][3][3];

public:
    Cube();
    virtual ~Cube();

    void init(void);
    void draw(void);
    void reset(void);

    //Get piece based on cube coordinates
    CubePiece*& getPiece(int x, int y, int z)    {return m_pPieces[x+1][y+1][z+1];};

    //Translate a mouse movement to a cube rotation.
    // glm::mat4, glm::vec3, glm::vec4, (x, y) | glm::vec2
    bool rotate(glm::mat4 Projection, glm::mat4 ModelView, glm::vec4 Viewport,
                glm::vec2 wndSize, glm::vec2 ptMouseWnd, glm::vec2 ptLastMouseWnd);
    void rotateXSection(int Section, bool bCW, bool bAnimate, bool bRecord);
    void rotateYSection(int Section, bool bCW, bool bAnimate, bool bRecord);
    void rotateZSection(int Section, bool bCW, bool bAnimate, bool bRecord);
protected:
    void animateRotation(CubePiece* piece[], int ctPieces, glm::vec3 v, float fAngle);
};

#endif //CUBE_H
