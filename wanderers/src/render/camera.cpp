/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Camera class.                                       *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "render/camera.h"

/* External Includes */
#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"

/* STL Includes */
#include <algorithm>

namespace wanderers {
namespace render {

Camera::Camera() : position_{kDefaultStartingPosition}, yaw_{kDefaultStartingYaw}, pitch_{kDefaultStartingPitch}, roll_{kDefaultStartingRoll},
                   direction_{ toDirection(yaw_, pitch_) },
                   field_of_view_{ kDefaultFieldOfView }, aspect_ratio_{ kDefaultAspectRatio }, near_{ kDefaultNear }, far_{kDefaultFar},
                   view_{glm::lookAt(position_, position_ + direction_, kUpVector)}, projection_{glm::perspective(field_of_view_, aspect_ratio_, near_, far_)},
                   mode_{kDefaultMode}, focus_id_{0} {}

Camera::Camera(glm::vec3 position, float yaw, float pitch, float roll) 
             : position_{ position }, yaw_{ yaw }, pitch_{ pitch }, roll_{ roll },
               direction_{ toDirection(yaw_, pitch_) },
               field_of_view_{ kDefaultFieldOfView }, aspect_ratio_{ kDefaultAspectRatio }, near_{ kDefaultNear }, far_{ kDefaultFar },
               view_{ glm::lookAt(position_, position_ + direction_, kUpVector) }, projection_{ glm::perspective(field_of_view_, aspect_ratio_, near_, far_) },
               mode_{ kDefaultMode }, focus_id_{0} {}

glm::vec3 Camera::getPosition() { return position_; }

/*
 * Camera getViewMatrix:
 * - Translate angles direction to vector direction.
 * - Create and return view matrix.
 */
glm::mat4 Camera::getViewMatrix() { 
    direction_ = toDirection(yaw_, pitch_);
    view_ = glm::lookAt(position_, position_ + direction_, kUpVector);
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
              +   glm::normalize(glm::cross(direction_, kUpVector)) * movement.x
              +   glm::normalize(glm::cross(glm::cross(direction_, kUpVector), direction_)) * movement.y;
}

/*
 * Camera turnYaw:
 * - Increments the yaw in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void Camera::turnYaw(float angle) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    yaw_ += angle;
    yaw_ = fmod(yaw_, 360.0f);
}

/*
 * Camera turnYaw:
 * - Increments the pitch in degrees but don't let it go below -89.0 or above 89.0 .
 */
void Camera::turnPitch(float angle) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    pitch_ += angle;
    pitch_ = std::max(std::min(pitch_, 89.0f), -89.0f);
}

/* Camera turnYaw:
 *  - Increments the roll in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void Camera::turnRoll(float angle) {
    std::lock_guard<std::mutex> guard(camera_mutex_);
    roll_ += angle;
    roll_ = fmod(roll_, 360.0f);
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

/*
 * Camera toDirection:
 * - Translate the angles from degrees to radians.
 * - Use trigonemtric functions to translate the angles to directional vector vector.
 */
glm::vec3 Camera::toDirection(float yaw, float pitch) {
    return glm::vec3{sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
                     sin(glm::radians(pitch)),
                     cos(glm::radians(yaw)) * cos(glm::radians(pitch))};
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

} // namespace render
} // namespace wanderers
