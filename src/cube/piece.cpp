#include "GL/glew.h"

#include "cube/piece.h"

namespace cube {

CubePiece::CubePiece(const glm::ivec3 &posHome) :
    m_posHome(posHome), m_fRotationAngle(0), m_vRotation(0,0,0)
{
    sideColors[Side::RIGHT]  = posHome.x== 1 ? colors::WHITE  : colors::BLACK;
    sideColors[Side::LEFT]   = posHome.x==-1 ? colors::BLUE   : colors::BLACK;
    sideColors[Side::TOP]    = posHome.y== 1 ? colors::GREEN  : colors::BLACK;
    sideColors[Side::BOTTOM] = posHome.y==-1 ? colors::ORANGE : colors::BLACK;
    sideColors[Side::FRONT]  = posHome.z== 1 ? colors::RED    : colors::BLACK;
    sideColors[Side::BACK]   = posHome.z==-1 ? colors::YELLOW : colors::BLACK;
}

void CubePiece::draw(int x, int y, int z) {
    glPushMatrix();

    if (m_fRotationAngle) {
        glRotatef(m_fRotationAngle, m_vRotation.x, m_vRotation.y, m_vRotation.z);
    }

    glTranslatef((float)x, (float)y, (float)z);

    glBegin(GL_QUADS);
    glColorSide(Side::TOP);
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);

    glColorSide(Side::BOTTOM);
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);

    glColorSide(Side::FRONT);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);

    glColorSide(Side::BACK);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f,-0.5f);

    glColorSide(Side::LEFT);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f,-0.5f);
    glVertex3f(-0.5f,-0.5f, 0.5f);

    glColorSide(Side::RIGHT);
    glVertex3f( 0.5f, 0.5f,-0.5f);
    glVertex3f( 0.5f, 0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f, 0.5f);
    glVertex3f( 0.5f,-0.5f,-0.5f);
    glEnd();

    glColor(colors::BLACK);

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

void CubePiece::rotateX(bool bCW) {
    glm::ivec3 nTmp;

    if (bCW) {
        nTmp                    = sideColors[Side::TOP];
        sideColors[Side::TOP]    = sideColors[Side::BACK];
        sideColors[Side::BACK]   = sideColors[Side::BOTTOM];
        sideColors[Side::BOTTOM] = sideColors[Side::FRONT];
        sideColors[Side::FRONT]  = nTmp;
    } else {
        nTmp                    = sideColors[Side::TOP];
        sideColors[Side::TOP]    = sideColors[Side::FRONT];
        sideColors[Side::FRONT]  = sideColors[Side::BOTTOM];
        sideColors[Side::BOTTOM] = sideColors[Side::BACK];
        sideColors[Side::BACK]   = nTmp;
    }
}

void CubePiece::rotateY(bool bCW) {
    glm::ivec3 nTmp;

    if (bCW) {
        nTmp                    = sideColors[Side::FRONT];
        sideColors[Side::FRONT]  = sideColors[Side::LEFT];
        sideColors[Side::LEFT]   = sideColors[Side::BACK];
        sideColors[Side::BACK]   = sideColors[Side::RIGHT];
        sideColors[Side::RIGHT]  = nTmp;
    } else {
        nTmp                    = sideColors[Side::FRONT];
        sideColors[Side::FRONT]  = sideColors[Side::RIGHT];
        sideColors[Side::RIGHT]  = sideColors[Side::BACK];
        sideColors[Side::BACK]   = sideColors[Side::LEFT];
        sideColors[Side::LEFT]   = nTmp;
    }
}

void CubePiece::rotateZ(bool bCW) {
    glm::ivec3 nTmp;

    if (bCW) {
        nTmp                    = sideColors[Side::TOP];
        sideColors[Side::TOP]    = sideColors[Side::RIGHT];
        sideColors[Side::RIGHT]  = sideColors[Side::BOTTOM];
        sideColors[Side::BOTTOM] = sideColors[Side::LEFT];
        sideColors[Side::LEFT]   = nTmp;
    } else {
        nTmp                    = sideColors[Side::TOP];
        sideColors[Side::TOP]    = sideColors[Side::LEFT];
        sideColors[Side::LEFT]   = sideColors[Side::BOTTOM];
        sideColors[Side::BOTTOM] = sideColors[Side::RIGHT];
        sideColors[Side::RIGHT]  = nTmp;
    }
}

void CubePiece::glColorSide(Side side) {
    auto color = sideColors[side];
    glColor(color);
}

void CubePiece::glColor(glm::ivec3 color) {
    glColor3ub(color.x, color.y, color.z);
}

}

