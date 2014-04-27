#ifndef ABSTRACT_CAMERA_HPP_WEXRRD3D
#define ABSTRACT_CAMERA_HPP_WEXRRD3D

#include "glm/glm.hpp"

#include "turn.hpp"
#include "direction.hpp"
#include "moving_status.hpp"

namespace gl {

const float PI = 3.14159265359;
const float HALF_PI = PI / 2.0;

const float DEFAULT_MOVE_SPEED = 3.f;
const float DEFAULT_TURN_SPEED = 0.01f;

class AbstractCamera {
protected:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up{0, 1, 0};

    unsigned xOrigin;
    unsigned yOrigin;

    float moveSpeed = DEFAULT_MOVE_SPEED;
    float turnSpeed = DEFAULT_TURN_SPEED;

    MovingStatus forwardBackward = MovingStatus::ZERO;
    MovingStatus rightLeft = MovingStatus::ZERO;
    MovingStatus upDown = MovingStatus::ZERO;

    MovingStatus turnX = MovingStatus::ZERO;
    MovingStatus turnY = MovingStatus::ZERO;
    MovingStatus turnZ = MovingStatus::ZERO;

public:
    AbstractCamera(int width, int height);
    virtual ~AbstractCamera() = default;

    virtual void update(float delta) = 0;

    virtual glm::mat4 getProjectionMatrix() const = 0;
    virtual glm::mat4 getViewMatrix() const = 0;

    void resize(int width, int height);

    void move(Direction direction);
    void stopMoving(Direction direction);

    void turn(Turn turn);
    void stopTurning(Turn turn);

    void setMoveSpeed(float speed);
    void setTurnSpeed(float speed);
};

// ------------------ //
// inline definitions //
// ------------------ //
inline AbstractCamera::AbstractCamera(int width, int height) {
    xOrigin = width >> 1;
    yOrigin = height >> 1;
}

inline void AbstractCamera::resize(int width, int height) {
    xOrigin = width >> 1;
    yOrigin = height >> 1;
}

inline void AbstractCamera::setMoveSpeed(float speed) {
    moveSpeed = speed;
}

inline void AbstractCamera::setTurnSpeed(float speed) {
    turnSpeed = speed;
}

}

#endif /* end of include guard: ABSTRACT_CAMERA_HPP_WEXRRD3D */

