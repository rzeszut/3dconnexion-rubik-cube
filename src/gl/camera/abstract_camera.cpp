#include "gl/camera/abstract_camera.hpp"

namespace gl {

void AbstractCamera::move(Direction direction) {
    switch (direction) {
    case Direction::FORWARD:
        forwardBackward = MovingStatus::PLUS;
        break;
    case Direction::BACKWARD:
        forwardBackward = MovingStatus::MINUS;
        break;

    case Direction::RIGHT:
        rightLeft = MovingStatus::PLUS;
        break;
    case Direction::LEFT:
        rightLeft = MovingStatus::MINUS;
        break;

    case Direction::UP:
        upDown = MovingStatus::PLUS;
        break;
    case Direction::DOWN:
        upDown = MovingStatus::MINUS;
        break;
    }
}

void AbstractCamera::stopMoving(Direction direction) {
    switch (direction) {
    case Direction::FORWARD:
    case Direction::BACKWARD:
        forwardBackward = MovingStatus::ZERO;
        break;

    case Direction::RIGHT:
    case Direction::LEFT:
        rightLeft = MovingStatus::ZERO;
        break;

    case Direction::UP:
    case Direction::DOWN:
        upDown = MovingStatus::ZERO;
        break;
    }
}

void AbstractCamera::turn(Turn turn) {
    switch (turn) {
    case Turn::X_INCREASE:
        turnX = MovingStatus::PLUS;
        break;
    case Turn::X_DECREASE:
        turnX = MovingStatus::MINUS;
        break;

    case Turn::Y_INCREASE:
        turnY = MovingStatus::PLUS;
        break;
    case Turn::Y_DECREASE:
        turnY = MovingStatus::MINUS;
        break;

    case Turn::Z_INCREASE:
        turnZ = MovingStatus::PLUS;
        break;
    case Turn::Z_DECREASE:
        turnZ = MovingStatus::MINUS;
        break;
    }
}

void AbstractCamera::stopTurning(Turn turn) {
    switch (turn) {
    case Turn::X_INCREASE:
    case Turn::X_DECREASE:
        turnX = MovingStatus::ZERO;
        break;

    case Turn::Y_INCREASE:
    case Turn::Y_DECREASE:
        turnY = MovingStatus::ZERO;
        break;

    case Turn::Z_INCREASE:
    case Turn::Z_DECREASE:
        turnZ = MovingStatus::ZERO;
        break;
    }
}

}

