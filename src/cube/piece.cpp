#include "GL/glew.h"

#include "cube/piece.h"


WCubePiece::WCubePiece(const glm::ivec3 &posHome) :
    m_posHome(posHome), m_fRotationAngle(0), m_vRotation(0,0,0)
{
    m_nSideColor[SD_RIGHT]  = posHome.x== 1 ? WHITE  : BLACK;
    m_nSideColor[SD_LEFT]   = posHome.x==-1 ? BLUE   : BLACK;
    m_nSideColor[SD_TOP]    = posHome.y== 1 ? GREEN  : BLACK;
    m_nSideColor[SD_BOTTOM] = posHome.y==-1 ? ORANGE : BLACK;
    m_nSideColor[SD_FRONT]  = posHome.z== 1 ? RED    : BLACK;
    m_nSideColor[SD_BACK]   = posHome.z==-1 ? YELLOW : BLACK;
}


WCubePiece::~WCubePiece()
{
}



void WCubePiece::draw(int x, int y, int z)
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





void WCubePiece::rotateX(bool bCW)
{
    SIDECOLOR nTmp;

    if (bCW) {
        nTmp                    = m_nSideColor[SD_TOP];
        m_nSideColor[SD_TOP]    = m_nSideColor[SD_BACK];
        m_nSideColor[SD_BACK]   = m_nSideColor[SD_BOTTOM];
        m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_FRONT];
        m_nSideColor[SD_FRONT]  = nTmp;
    }
    else {
        nTmp                    = m_nSideColor[SD_TOP];
        m_nSideColor[SD_TOP]    = m_nSideColor[SD_FRONT];
        m_nSideColor[SD_FRONT]  = m_nSideColor[SD_BOTTOM];
        m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_BACK];
        m_nSideColor[SD_BACK]   = nTmp;
    }
}



void WCubePiece::rotateY(bool bCW)
{
    SIDECOLOR nTmp;

    if (bCW) {
        nTmp                    = m_nSideColor[SD_FRONT];
        m_nSideColor[SD_FRONT]  = m_nSideColor[SD_LEFT];
        m_nSideColor[SD_LEFT]   = m_nSideColor[SD_BACK];
        m_nSideColor[SD_BACK]   = m_nSideColor[SD_RIGHT];
        m_nSideColor[SD_RIGHT]  = nTmp;
    }
    else {
        nTmp                    = m_nSideColor[SD_FRONT];
        m_nSideColor[SD_FRONT]  = m_nSideColor[SD_RIGHT];
        m_nSideColor[SD_RIGHT]  = m_nSideColor[SD_BACK];
        m_nSideColor[SD_BACK]   = m_nSideColor[SD_LEFT];
        m_nSideColor[SD_LEFT]   = nTmp;
    }
}



void WCubePiece::rotateZ(bool bCW)
{
    SIDECOLOR nTmp;

    if (bCW) {
        nTmp                    = m_nSideColor[SD_TOP];
        m_nSideColor[SD_TOP]    = m_nSideColor[SD_RIGHT];
        m_nSideColor[SD_RIGHT]  = m_nSideColor[SD_BOTTOM];
        m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_LEFT];
        m_nSideColor[SD_LEFT]   = nTmp;
    }
    else {
        nTmp                    = m_nSideColor[SD_TOP];
        m_nSideColor[SD_TOP]    = m_nSideColor[SD_LEFT];
        m_nSideColor[SD_LEFT]   = m_nSideColor[SD_BOTTOM];
        m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_RIGHT];
        m_nSideColor[SD_RIGHT]  = nTmp;
    }
}
