#ifndef PIECE_H
#define PIECE_H

#include <map>

#include "glm/glm.hpp"

#include "cube/util.h"

namespace cube {

enum class Side: short {
    RIGHT,
    LEFT,
    TOP,
    BOTTOM,
    FRONT,
    BACK,
};

namespace colors {
    const glm::ivec3 RED    {255, 0, 0};
    const glm::ivec3 GREEN  {0, 255, 0};
    const glm::ivec3 BLUE   {0, 0, 255};
    const glm::ivec3 ORANGE {255, 127, 0};
    const glm::ivec3 YELLOW {255, 255, 0};
    const glm::ivec3 WHITE  {255, 255, 255};
    const glm::ivec3 BLACK  {0, 0, 0};
};

class CubePiece {
private:
    std::map<Side, glm::ivec3> sideColors;
    glm::ivec3 m_posHome;

    //Rotation animation state variables
    float m_fRotationAngle;
    glm::vec3 m_vRotation;

public:
    CubePiece(const glm::ivec3 &pos);
    ~CubePiece() = default;

    void draw(int x, int y, int z);

    void rotateX(bool clockwise);
    void rotateY(bool clockwise);
    void rotateZ(bool clockwise);

    void setRotation(float fAngle, const glm::vec3 &vRotation) {
        m_fRotationAngle = fAngle;
        m_vRotation = vRotation;
    }

    void clrRotation() {
        m_fRotationAngle = 0;
    }

private:
    void glColorSide(Side side);
    void glColor(glm::ivec3 color);
};

}

#endif //PIECE_H
