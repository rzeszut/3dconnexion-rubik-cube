#ifndef PIECE_H
#define PIECE_H


#include "util.h"

// use glm
#include "vec3f.h"

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
   BYTE r, g, b;
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

#define MAKECOLOR(nColor)        m_rgb[nColor].r, m_rgb[nColor].g, m_rgb[nColor].b
#define MAKESIDECOLOR(nSide)     MAKECOLOR(m_nSideColor[nSide])


// remove this
class BYTEVEC 
{
public:
   INT8 x,y,z;

public:
   BYTEVEC() {};
   BYTEVEC(INT8 xx, INT8 yy, INT8 zz)   {x=xx; y=yy; z=zz;};
};




class WCubePiece
{
protected:
   SIDECOLOR m_nSideColor[6];
   BYTEVEC m_posHome;        
                             

   //Rotation animation state variables
   float m_fRotationAngle;     
   Vec3f m_vRotation;     


public:
   WCubePiece(BYTEVEC pos);
   virtual ~WCubePiece();

   void draw(INT8 x, INT8 y, INT8 z);

   void rotateX(BOOL bCW);
   void rotateY(BOOL bCW);
   void rotateZ(BOOL bCW);

   void setRotation(float fAngle, Vec3f vRotation)       {m_fRotationAngle=fAngle; m_vRotation=vRotation;};
   void clrRotation(void)                                {m_fRotationAngle=0;};
};


#endif //PIECE_H
