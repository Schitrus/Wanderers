/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the CameraObject class.                                 *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "simulation/object/camera_object.h"

/* External Includes */
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/vector_angle.hpp"

/* STL Includes */
#include <algorithm>

#include <iostream>

namespace wanderers {
namespace simulation {
namespace object {

CameraObject::CameraObject() : position_{ kDefaultPosition }, direction_{ kDefaultDirection }, 
                               up_{ kDefaultUpVector }, right_{glm::cross(direction_, up_)} {}

CameraObject::CameraObject(glm::vec3 position, glm::vec3 direction, glm::vec3 up) 
                          : position_{ position }, direction_{ glm::normalize(direction) },
                            right_{glm::normalize(glm::cross(direction_, up))}, up_{ glm::cross(right_, direction_) } {}

glm::vec3 CameraObject::getPosition() { return position_; }
glm::vec3 CameraObject::getDirection() { return direction_; }
glm::vec3 CameraObject::getUp() { return up_; }
glm::vec3 CameraObject::getRight() { return right_; }


void CameraObject::setPosition(glm::vec3 position) {
    position_ = position;
}

void CameraObject::setDirection(glm::vec3 direction) {
    glm::mat4 rotation{ glm::orientation(direction_, glm::normalize(direction)) };
    direction_ = glm::normalize(direction);
    up_ = rotation * glm::vec4{ up_, 0.0f };
    right_ = rotation * glm::vec4{ right_, 0.0f };
}

void CameraObject::setUp(glm::vec3 up) {
    glm::mat4 rotation{ glm::orientation(up_, glm::normalize(up)) };
    right_ = rotation * glm::vec4{ right_, 0.0f };
    up_ = glm::cross(right_, direction_);
}

void CameraObject::setRight(glm::vec3 right) {
    glm::mat4 rotation{ glm::orientation(right_, glm::normalize(right)) };
    up_ = rotation * glm::vec4{ up_, 0.0f };
    right_ = glm::cross(direction_, up_);
}


/*
 * CameraObject move:
 * - Increment the position with the movement in z direction parallel with the direction vector.
 * - Increment the position with the movement in x direction orthogonal to the direction and y vector.
 * - Increment the position with the movement in y direction orthogonal to the direction and cameras up direction.
 */
void CameraObject::move(glm::vec3 movement) {
    position_ +=  direction_ * movement.z
              +   up_        * movement.y
              +   right_     * movement.x;
}

void CameraObject::translate(glm::vec3 translation) {
    position_ += translation;
}

/*
 * CameraObject turnYaw:
 * - Increments the yaw in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void CameraObject::turnYaw(float yaw) {
    direction_ = glm::rotate(direction_, glm::radians(yaw), up_);
    right_     = glm::rotate(right_, glm::radians(yaw), up_);
}

/*
 * CameraObject turnPitch:
 * - Increments the pitch in degrees but don't let it go below -89.0 or above 89.0 .
 */
void CameraObject::turnPitch(float pitch) {
    direction_ = glm::rotate(direction_, glm::radians(pitch), glm::cross(direction_, up_));
    up_        = glm::rotate(up_, glm::radians(pitch), glm::cross(direction_, up_));
}

/* CameraObject turnRoll:
 *  - Increments the roll in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void CameraObject::turnRoll(float roll) {
    up_    = glm::rotate(up_, glm::radians(roll), direction_);
    right_ = glm::rotate(right_, glm::radians(roll), direction_);
}

} // namespace object
} // namespace simulation
} // namespace wanderers
