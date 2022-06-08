/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the Camera view class.                                  *
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

CameraView::CameraView() : field_of_view_{ kDefaultFieldOfView }, aspect_ratio_{ kDefaultAspectRatio }, near_{ kDefaultNear }, far_{kDefaultFar} {}
CameraView::CameraView(float field_of_view, float aspect_ratio, float near, float far) 
                      : field_of_view_{ field_of_view }, aspect_ratio_{ aspect_ratio }, near_{ near }, far_{ far } {}

float CameraView::getFieldOfView() {
    return field_of_view_;
}
float CameraView::getAspectRatio() {
    return aspect_ratio_;
}
float CameraView::getNear() {
    return near_;
}
float CameraView::getFar() {
    return far_;
}

void CameraView::setFieldOfView(float field_of_view) {
    field_of_view_ = field_of_view;
}

void CameraView::setAspectRatio(float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
}
void CameraView::setAspectRatio(int width, int height) {
    aspect_ratio_ = static_cast<float>(width) / height;
}

void CameraView::setNear(float near) {
    near_ = near;
}

void CameraView::setFar(float far) {
    far_ = far;
}

void CameraView::withMutext(std::function<void(void)> func) {
    assert(func);
    std::lock_guard<std::mutex> guard{ camera_view_mutex_ };
    func();
}

} // namespace render
} // namespace wanderers
