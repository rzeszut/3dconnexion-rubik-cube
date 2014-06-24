#ifndef PIECE_H
#define PIECE_H

#include "util.h"
#include "glm/glm.hpp"

typedef enum {
    SD_RIGHT,
    SD_LEFT,
    SD_TOP,
    SD_BOTTOM,
    SD_FRONT,
    SD_BACK,
} SIDE;

//Define colors for side
typedef enum {
    RED,
    GREEN,
    BLUE,
    PURPLE,
    ORANGE,
    YELLOW,
    BLACK,
    WHITE,
} SIDECOLOR;

const struct {
    unsigned char r, g, b;
} m_rgb[] = {
#ifndef USE_SCOTT_COLORS
    {255,  0,  0,},              //Red
    {  0, 255,   0,},            //Green
    {  0,   0, 255,},            //Blue
    {255,   0, 255,},            //Purple
    {255, 127,   0,},            //Orange
    {255, 255,   0,},            //Yellow
    {  0,   0,   0,},            //Black
    {255, 255, 255,},            //White
#else
    { 190,  42,  63,},            //Red
    {   2, 124,   3,},            //Green
    {  21,  62, 152,},            //Blue
    { 127,   0, 127,},            //Purple
    { 237, 130,  94,},            //Orange
    { 238, 208,  78,},            //Yellow
    {   0,   0,   0,},            //Black
    { 255, 255, 255,},            //White
#endif //USE_SCOTT_COLORS
};

#define MAKECOLOR(Color)        m_rgb[Color].r, m_rgb[Color].g, m_rgb[Color].b
#define MAKESIDECOLOR(Side)     MAKECOLOR(m_SideColor[Side])

class CubePiece
{
protected:
    SIDECOLOR m_SideColor[6];
    glm::ivec3 m_posHome;


    //Rotation animation state variables
    float m_fRotationAngle;
    glm::vec3 m_vRotation;


public:
    CubePiece(const glm::ivec3 &pos);
    virtual ~CubePiece();

    void draw(int x, int y, int z);

    void rotateX(bool bCW);
    void rotateY(bool bCW);
    void rotateZ(bool bCW);

    void setRotation(float fAngle, const glm::vec3 &vRotation)       {m_fRotationAngle=fAngle; m_vRotation=vRotation;};
    void clrRotation(void)                                {m_fRotationAngle=0;};
};


#endif //PIECE_H
