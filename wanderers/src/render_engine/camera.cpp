#include "render_engine/camera.h"

#include "glm/ext.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

Camera::Camera() : position_{kDefaultStartingPosition}, yaw_{kDefaultStartingYaw}, pitch_{kDefaultStartingPitch}, roll_{kDefaultStartingRoll},
                   direction_{ toDirection(yaw_, pitch_) },
                   field_of_view_{ kDefaultFieldOfView }, aspect_ratio_{ kDefaultAspectRatio }, near_{ kDefaultNear }, far_{kDefaultFar},
                   view_{glm::lookAt(position_, position_ + direction_, kUpVector)}, projection_{glm::perspective(field_of_view_, aspect_ratio_, near_, far_)} {}

Camera::Camera(glm::vec3 position, float yaw, float pitch, float roll) 
             : position_{ position }, yaw_{ yaw }, pitch_{ pitch }, roll_{ roll },
               direction_{ toDirection(yaw_, pitch_) },
               field_of_view_{ kDefaultFieldOfView }, aspect_ratio_{ kDefaultAspectRatio }, near_{ kDefaultNear }, far_{ kDefaultFar },
               view_{ glm::lookAt(position_, position_ + direction_, kUpVector) }, projection_{ glm::perspective(field_of_view_, aspect_ratio_, near_, far_) } {}

glm::vec3 Camera::getPosition() { return position_; }

glm::mat4 Camera::getViewMatrix() { 
    direction_ = toDirection(yaw_, pitch_);
    view_ = glm::lookAt(position_, position_ + direction_, kUpVector);
    return view_;
}

glm::mat4 Camera::getProjectionMatrix() { 
    projection_ = glm::perspective(glm::radians(field_of_view_), aspect_ratio_, near_, far_);
    return projection_;
}

void Camera::move(glm::vec3 movement) { 
    position_ += -glm::normalize(direction_) * movement.z
        + glm::normalize(glm::cross(direction_, kUpVector)) * movement.x
        + glm::normalize(glm::cross(glm::cross(direction_, kUpVector), direction_)) * movement.y;
}

void Camera::turnYaw(float angle) {
    yaw_ += angle;
    yaw_ = fmod(yaw_, 360.0f);
}

void Camera::turnPitch(float angle) {
    pitch_ += angle;
    pitch_ = std::max(std::min(pitch_, 89.0f), -89.0f);
}

void Camera::turnRoll(float angle) {
    roll_ += angle;
    roll_ = fmod(roll_, 360.0f);
}

void Camera::setPosition(glm::vec3 position) { position_ = position; }

void Camera::setAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; }
void Camera::setAspectRatio(int width, int height) {
    aspect_ratio_ = static_cast<float>(width) / height;
}

glm::vec3 Camera::toDirection(float yaw, float pitch) {
    return glm::vec3{sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
                     sin(glm::radians(pitch)),
                     cos(glm::radians(yaw)) * cos(glm::radians(pitch))};
}