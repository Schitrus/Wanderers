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
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/vector_angle.hpp"

/* STL Includes */
#include <algorithm>
#include <mutex>

#include <iostream>

namespace wanderers {
namespace render {

Camera::Camera() : CameraObject{}, CameraView{} {}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float field_of_view, float aspect_ratio, float near, float far) 
              : CameraObject{position, direction, up}, CameraView{field_of_view, aspect_ratio, near, far} {}

/*
 * Camera getViewMatrix:
 * - Translate angles direction to vector direction.
 * - Create and return view matrix.
 */
glm::mat4 Camera::getViewMatrix() {
    view_ = glm::lookAt(position_, position_ + getDirection(), getUp());
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

void Camera::lock() {
    camera_object_mutex_.lock();
    camera_view_mutex_.lock();
}

void Camera::unlock() {
    camera_view_mutex_.unlock();
    camera_object_mutex_.unlock();
}

void Camera::withMutext(std::function<void(void)> func) {
    assert(func);
    std::lock_guard<std::mutex> view_guard{ camera_view_mutex_ };
    std::lock_guard<std::mutex> object_guard{ camera_object_mutex_ };
    func();
}

bool Camera::shouldClear() {
    return getShouldClear();
}
void Camera::setShouldClear(bool should_clear) {
    should_clear_ = should_clear;
}
bool Camera::getShouldClear() {
    return should_clear_;
}

} // namespace render
} // namespace wanderers
