/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Camera class.                                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "render/camera_view.h"

/* External Includes */
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/vector_angle.hpp"

/* STL Includes */
#include <algorithm>

#include <iostream>

namespace wanderers {
namespace render {

Camera::Camera() : position_{kDefaultStartingPosition}, direction_{ kDefaultDirection }, up_vector_{ kDefaultUpVector }, relative_direction_{0.0f, 0.0f, 1.0f},
                   field_of_view_{ kDefaultFieldOfView }, aspect_ratio_{ kDefaultAspectRatio }, near_{ kDefaultNear }, far_{kDefaultFar},
                   view_{glm::lookAt(position_, position_ + direction_, up_vector_)}, projection_{glm::perspective(field_of_view_, aspect_ratio_, near_, far_)},
                   mode_{kDefaultMode}, focus_id_{0} {}

Camera::Camera(glm::vec3 position, glm::vec3 direction) 
             : position_{ position }, direction_{ direction }, up_vector_{ kDefaultUpVector }, relative_direction_{ 0.0f, 0.0f, 1.0f },
               field_of_view_{ kDefaultFieldOfView }, aspect_ratio_{ kDefaultAspectRatio }, near_{ kDefaultNear }, far_{ kDefaultFar },
               view_{ glm::lookAt(position_, position_ + direction_, up_vector_) }, projection_{ glm::perspective(field_of_view_, aspect_ratio_, near_, far_) },
               mode_{ kDefaultMode }, focus_id_{0} {}

glm::vec3 Camera::getPosition() { return position_; }

/*
 * Camera getViewMatrix:
 * - Translate angles direction to vector direction.
 * - Create and return view matrix.
 */
glm::mat4 Camera::getViewMatrix() { 
    view_ = glm::lookAt(position_, position_ + direction_, up_vector_);
    return view_;
}

/*
 * Camera getProjectionMatrix
 * - Create and return perspective projection matrix.
 */
glm::mat4 Camera::getProjectionMatrix() { 
    projection_ = glm::perspective(glm::radians(field_of_view_), aspect_ratio_, near_, far_);
    return projection_;
}

/*
 * Camera move:
 * - Increment the position with the movement in z direction parallel with the direction vector.
 * - Increment the position with the movement in x direction orthogonal to the direction and y vector.
 * - Increment the position with the movement in y direction orthogonal to the direction and cameras up direction.
 */
void Camera::move(glm::vec3 movement) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    position_ += -glm::normalize(direction_) * movement.z
              +   glm::normalize(up_vector_) * movement.y
              +   glm::normalize(glm::cross(direction_, up_vector_)) * movement.x;
}

/*
 * Camera turnYaw:
 * - Increments the yaw in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void Camera::turnYaw(float yaw) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    direction_ = glm::rotate(direction_, glm::radians(yaw), up_vector_);
}

/*
 * Camera turnPitch:
 * - Increments the pitch in degrees but don't let it go below -89.0 or above 89.0 .
 */
void Camera::turnPitch(float pitch) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    direction_ = glm::rotate(direction_, glm::radians(pitch), glm::cross(direction_, up_vector_));
    up_vector_ = glm::rotate(up_vector_, glm::radians(pitch), glm::cross(direction_, up_vector_));
}

/* Camera turnRoll:
 *  - Increments the roll in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void Camera::turnRoll(float roll) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    up_vector_ = glm::rotate(up_vector_, glm::radians(roll), direction_);
}

void Camera::setPosition(glm::vec3 position) { 
    std::lock_guard<std::mutex> guard(camera_mutex_);
    position_ = position; 
}

void Camera::setAspectRatio(float aspect_ratio) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    aspect_ratio_ = aspect_ratio;
}
void Camera::setAspectRatio(int width, int height) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    aspect_ratio_ = static_cast<float>(width) / height;
}

void Camera::lock() {
    camera_mutex_.lock();
}

void Camera::unlock() {
    camera_mutex_.unlock();
}

Camera::Mode Camera::getMode() {
    return mode_;
}

void Camera::setMode(Camera::Mode mode) {
    mode_ = mode;
}

Camera::Mode Camera::cycleMode() {
    mode_ = static_cast<Camera::Mode>((static_cast<int>(mode_) + 1) % static_cast<int>(Camera::Mode::Count));
    return mode_;
}

unsigned int Camera::getFocusId() {
    return focus_id_;
}

void Camera::setFocusId(unsigned int focus_id) {
    focus_id_ = focus_id;
}

glm::vec3 Camera::getDirection() {
    return direction_;
}

void Camera::setDirection(glm::vec3 direction) {
    direction_ = direction;
}

void Camera::setRelativeDirection(glm::vec3 relative_direction) {
    float angle{ glm::angle(glm::normalize(relative_direction_), glm::normalize(relative_direction)) };
    if (angle > 0.0f) {
        glm::vec3 rotational_axis{ glm::normalize(cross(glm::normalize(relative_direction_), glm::normalize(relative_direction))) };
        direction_ = glm::rotate(glm::normalize(direction_), angle, rotational_axis);
        up_vector_ = glm::rotate(glm::normalize(up_vector_), angle, rotational_axis);
    }

    relative_direction_ = glm::normalize(relative_direction);
}

} // namespace render
} // namespace wanderers
