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
    view_ = glm::lookAt(position_, position_ + direction_, up_);
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

} // namespace render
} // namespace wanderers
