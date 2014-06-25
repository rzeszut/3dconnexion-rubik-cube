#ifndef CUBE_H
#define CUBE_H

#include "glm/glm.hpp"

#include "util/optional.hpp"

#include "util.h"
#include "piece.h"

namespace cube {

enum class Rotation {
    X_ROTATION,
    Y_ROTATION
};

enum class Clockwise : bool {
    Clockwise = true,
    CounterClockwise = false
};

inline Clockwise invert(Clockwise c) {
    return (bool) c ? Clockwise::CounterClockwise : Clockwise::Clockwise;
}

constexpr int ROTATION_FRAMES = 30;

class Cube {
private:
    CubePiece* m_pPieces[3][3][3];

    // rotation data
    bool rotating;

    Rotation rotation;
    Clockwise clockwise;
    SIDE side;
    int section;

    int rotationIteration;

public:
    Cube();
    virtual ~Cube();

    void init(void);
    void reset(void);

    void update();
    void draw(void);

    bool isRotating() const {
        return rotating;
    }

    //Get piece based on cube coordinates
    CubePiece*& getPiece(int x, int y, int z) {
        return m_pPieces[x+1][y+1][z+1];
    }

    // Translate a mouse movement to a cube rotation.
    bool rotate(glm::mat4 Projection, glm::mat4 ModelView, glm::vec4 Viewport,
                glm::vec2 wndSize, glm::vec2 ptMouseWnd, glm::vec2 ptLastMouseWnd);

private:
    void updateXRotation();
    void updateYRotation();

    void updateXSection(Clockwise modifier);
    void updateYSection(Clockwise modifier);
    void updateZSection(Clockwise modifier);

    bool animate(CubePiece *pieces[9], const glm::vec3 &axis, Clockwise modifier);
};

}

#endif //CUBE_H
