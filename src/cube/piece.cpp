#include "GL/glew.h"

#include "cube/piece.h"


CubePiece::CubePiece(const glm::ivec3 &posHome) :
    m_posHome(posHome), m_fRotationAngle(0), m_vRotation(0,0,0)
{
    m_SideColor[SD_RIGHT]  = posHome.x== 1 ? WHITE  : BLACK;
    m_SideColor[SD_LEFT]   = posHome.x==-1 ? BLUE   : BLACK;
    m_SideColor[SD_TOP]    = posHome.y== 1 ? GREEN  : BLACK;
    m_SideColor[SD_BOTTOM] = posHome.y==-1 ? ORANGE : BLACK;
    m_SideColor[SD_FRONT]  = posHome.z== 1 ? RED    : BLACK;
    m_SideColor[SD_BACK]   = posHome.z==-1 ? YELLOW : BLACK;
}


CubePiece::~CubePiece()
{
}



void CubePiece::draw(int x, int y, int z)
{
    glPushMatrix();
    if (m_fRotationAngle)
        glRotatef(m_fRotationAngle, m_vRotation.x, m_vRotation.y, m_vRotation.z);

    glTranslatef((float)x, (float)y, (float)z);

    glBegin(GL_QUADS);
    glColor3ub(MAKESIDECOLOR(SD_TOP));
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);

    glColor3ub(MAKESIDECOLOR(SD_BOTTOM));
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);

    glColor3ub(MAKESIDECOLOR(SD_FRONT));
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);

    glColor3ub(MAKESIDECOLOR(SD_BACK));
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f,-0.5f);

    glColor3ub(MAKESIDECOLOR(SD_LEFT));
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);

    glColor3ub(MAKESIDECOLOR(SD_RIGHT));
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glEnd();

    glColor3ub(MAKECOLOR(BLACK));

    glBegin(GL_LINE_STRIP);
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glEnd();

    glPopMatrix();
}





void CubePiece::rotateX(bool bCW)
{
    SIDECOLOR nTmp;

    if (bCW) {
        nTmp                    = m_SideColor[SD_TOP];
        m_SideColor[SD_TOP]    = m_SideColor[SD_BACK];
        m_SideColor[SD_BACK]   = m_SideColor[SD_BOTTOM];
        m_SideColor[SD_BOTTOM] = m_SideColor[SD_FRONT];
        m_SideColor[SD_FRONT]  = nTmp;
    }
    else {
        nTmp                    = m_SideColor[SD_TOP];
        m_SideColor[SD_TOP]    = m_SideColor[SD_FRONT];
        m_SideColor[SD_FRONT]  = m_SideColor[SD_BOTTOM];
        m_SideColor[SD_BOTTOM] = m_SideColor[SD_BACK];
        m_SideColor[SD_BACK]   = nTmp;
    }
}



void CubePiece::rotateY(bool bCW)
{
    SIDECOLOR nTmp;

    if (bCW) {
        nTmp                    = m_SideColor[SD_FRONT];
        m_SideColor[SD_FRONT]  = m_SideColor[SD_LEFT];
        m_SideColor[SD_LEFT]   = m_SideColor[SD_BACK];
        m_SideColor[SD_BACK]   = m_SideColor[SD_RIGHT];
        m_SideColor[SD_RIGHT]  = nTmp;
    }
    else {
        nTmp                    = m_SideColor[SD_FRONT];
        m_SideColor[SD_FRONT]  = m_SideColor[SD_RIGHT];
        m_SideColor[SD_RIGHT]  = m_SideColor[SD_BACK];
        m_SideColor[SD_BACK]   = m_SideColor[SD_LEFT];
        m_SideColor[SD_LEFT]   = nTmp;
    }
}



void CubePiece::rotateZ(bool bCW)
{
    SIDECOLOR nTmp;

    if (bCW) {
        nTmp                    = m_SideColor[SD_TOP];
        m_SideColor[SD_TOP]    = m_SideColor[SD_RIGHT];
        m_SideColor[SD_RIGHT]  = m_SideColor[SD_BOTTOM];
        m_SideColor[SD_BOTTOM] = m_SideColor[SD_LEFT];
        m_SideColor[SD_LEFT]   = nTmp;
    }
    else {
        nTmp                    = m_SideColor[SD_TOP];
        m_SideColor[SD_TOP]    = m_SideColor[SD_LEFT];
        m_SideColor[SD_LEFT]   = m_SideColor[SD_BOTTOM];
        m_SideColor[SD_BOTTOM] = m_SideColor[SD_RIGHT];
        m_SideColor[SD_RIGHT]  = nTmp;
    }
}
