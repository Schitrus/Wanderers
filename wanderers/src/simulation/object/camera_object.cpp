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
#include <iomanip>

namespace wanderers {
namespace simulation {
namespace object {

/// MAKE IT HAVE ABSTRACT OBJECT, NOT INHERIT

CameraObject::CameraObject() : CameraObject{ kDefaultPosition, kDefaultDirection, kDefaultUpVector }  {}

CameraObject::CameraObject(glm::vec3 position, glm::vec3 direction, glm::vec3 up) 
                          : speed_{1.0f}, position_{ position }, relative_position_{0.0f}, direction_{ glm::normalize(direction) },
                            right_{glm::normalize(glm::cross(direction_, up))}, up_{ glm::cross(right_, direction_) },
                            relative_direction_{0.0f, 0.0f,-1.0f}, relative_up_{0.0f, 1.0f, 0.0f}, relative_right_{1.0f, 0.0f, 0.0f},
                            camera_mode_{ kDefaultCameraMode }, camera_focus_{ nullptr } {}

glm::vec3 CameraObject::getPosition() { 
    return position_; 
}
glm::vec3 CameraObject::getDirection() { 
    return direction_;
}
glm::vec3 CameraObject::getUp() { 
    return up_; 
}
glm::vec3 CameraObject::getRight() { 
    return right_; 
}


void CameraObject::setPosition(glm::vec3 position) {
    position_ = position;
}

void CameraObject::setDirection(glm::vec3 direction) {
    if (direction == direction_)
        return;
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
    if (camera_mode_ == CameraMode::Center) {
        //std::cout << std::setprecision(16) <<"---movement: " << movement.x << ", " << movement.y << ", " << movement.z << std::endl;
        //std::cout << "-->RelativePosition: " << relative_position_.x << ", " << relative_position_.y << ", " << relative_position_.z << std::endl;
        relative_position_ += (glm::rotate(relative_position_, movement.x, relative_up_) - relative_position_)
                           +  (glm::rotate(relative_position_, movement.y, relative_right_) - relative_position_)
                           -   glm::normalize(relative_position_) * movement.z * speed_ * glm::length(relative_position_) / 10.0f;
        //std::cout << "-->RelativePosition: " << relative_position_.x << ", " << relative_position_.y << ", " << relative_position_.z << std::endl;

        if (glm::all(glm::equal(relative_direction_, glm::normalize(relative_position_)))) {
            relative_direction_ = -glm::normalize(relative_position_);
            relative_right_ = -relative_right_;

        }
        else if (glm::any(glm::notEqual(relative_direction_, -glm::normalize(relative_position_)))) {
            glm::vec3 normal = glm::cross(relative_direction_, -glm::normalize(relative_position_));
            float angle = glm::angle(relative_direction_, -glm::normalize(relative_position_));
            //std::cout << "->Relative Direction: " << relative_direction_.x << ", " << relative_direction_.y << ", " << relative_direction_.z << std::endl;
            //std::cout << "->normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
            //std::cout << "->angle: " << angle << std::endl;
            if (normal != glm::vec3{ 0.0f } && angle != 0.0f) {
                relative_direction_ = glm::normalize(glm::rotate(relative_direction_, angle, normal));
                relative_up_ = glm::normalize(glm::rotate(relative_up_, angle, normal));
                relative_right_ = glm::normalize(glm::cross(relative_direction_, relative_up_));
            }
            //std::cout << "<-Relative Direction: " << relative_direction_.x << ", " << relative_direction_.y << ", " << relative_direction_.z << std::endl << std::endl;
        }
    } else if (camera_mode_ == CameraMode::Orbital) {
        relative_position_ += direction_ * movement.z * speed_
                           +  up_        * movement.y * speed_
                           +  right_     * movement.x * speed_;
    } else if (camera_mode_ == CameraMode::Rotational) {
        relative_position_ += relative_direction_ * movement.z * speed_
                           +  relative_up_        * movement.y * speed_
                           +  relative_right_     * movement.x * speed_;
    } else {
        position_ +=  direction_ * movement.z * speed_
                  +   up_        * movement.y * speed_
                  +   right_     * movement.x * speed_;
    }
}

void CameraObject::translate(glm::vec3 translation) {
    switch (camera_mode_) {
    case CameraMode::Orbital:
        relative_position_ += translation;
        break;
    default:
        position_ += translation;
        break;
    }
}

/*
 * CameraObject turnYaw:
 * - Increments the yaw in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void CameraObject::turnYaw(float yaw) {
    if (camera_mode_ == CameraMode::Rotational) {
        relative_direction_ = glm::rotate(relative_direction_, glm::radians(yaw), relative_up_);
        relative_right_ = glm::rotate(relative_right_, glm::radians(yaw), relative_up_);
    } else {
        direction_ = glm::rotate(direction_, glm::radians(yaw), up_);
        right_     = glm::rotate(right_, glm::radians(yaw), up_);
    }
}

/*
 * CameraObject turnPitch:
 * - Increments the pitch in degrees but don't let it go below -89.0 or above 89.0 .
 */
void CameraObject::turnPitch(float pitch) {
    if (camera_mode_ == CameraMode::Rotational) {
        relative_direction_ = glm::rotate(relative_direction_, glm::radians(pitch), glm::cross(relative_direction_, relative_up_));
        relative_up_        = glm::rotate(relative_up_, glm::radians(pitch), glm::cross(relative_direction_, relative_up_));
    } else {
        direction_ = glm::rotate(direction_, glm::radians(pitch), glm::cross(direction_, up_));
        up_ = glm::rotate(up_, glm::radians(pitch), glm::cross(direction_, up_));
    }

}

/* CameraObject turnRoll:
 *  - Increments the roll in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void CameraObject::turnRoll(float roll) {
    if (camera_mode_ == CameraMode::Rotational) {
        relative_up_    = glm::rotate(relative_up_, glm::radians(roll), relative_direction_);
        relative_right_ = glm::rotate(relative_right_, glm::radians(roll), relative_direction_);
    } else {
        up_ = glm::rotate(up_, glm::radians(roll), direction_);
        right_ = glm::rotate(right_, glm::radians(roll), direction_);
    }
}

CameraObject::CameraMode CameraObject::getCameraMode() {
    return camera_mode_;
}


void CameraObject::setCameraMode(CameraObject::CameraMode mode) {
    modeUpdate(mode, camera_focus_);
    camera_mode_ = mode;
}

CameraObject::CameraMode CameraObject::cycleCameraMode() {
    setCameraMode(static_cast<CameraObject::CameraMode>((static_cast<int>(camera_mode_) + 1) % static_cast<int>(CameraObject::CameraMode::Count)));
    return camera_mode_;
}

glm::mat4 getFocusMatrix(AbstractObject* focus) {
    AbstractObject* parent{ focus->getParent() };
    glm::mat4 matrix{ 1.0f };
    while (parent != nullptr) {
        matrix = parent->getMatrix() * matrix;
        parent = parent->getParent();
    }
    return matrix;
}

void CameraObject::elapseTime(double seconds) {
    if (camera_mode_ == CameraMode::Center) {
        AbstractObject* parent{ camera_focus_->getParent() };
        glm::mat4 world_matrix{ 1.0f };
        while (parent != nullptr) {
            world_matrix = parent->getMatrix() * world_matrix;
            parent = parent->getParent();
        }

        position_ = world_matrix * glm::vec4{ relative_position_, 1.0f };

        direction_ = world_matrix * glm::vec4{ relative_direction_, 0.0f };
        up_ = world_matrix * glm::vec4{ relative_up_, 0.0f };
        right_ = glm::normalize(glm::cross(direction_, up_));
    } else if (camera_mode_ == CameraMode::Orbital){
        AbstractObject* parent{ camera_focus_->getParent() };
        glm::mat4 world_matrix{ 1.0f };
        while (parent != nullptr) {
            world_matrix = parent->getMatrix() * world_matrix;
            parent = parent->getParent();
        }
        position_ = world_matrix * glm::vec4{ relative_position_, 1.0f };
    } else if (camera_mode_ == CameraMode::Rotational) {
        AbstractObject* parent{ camera_focus_->getParent() };
        glm::mat4 world_matrix{ 1.0f };
        while (parent != nullptr) {
            world_matrix = parent->getMatrix() * world_matrix;
            parent = parent->getParent();
        }
        position_ = world_matrix * camera_focus_->getMatrix() * glm::vec4{relative_position_, 1.0f};

        direction_ = world_matrix * camera_focus_->getMatrix() * glm::vec4{ relative_direction_, 0.0f };
        up_ = world_matrix * camera_focus_->getMatrix() * glm::vec4{ relative_up_, 0.0f };
        right_ = glm::normalize(glm::cross(direction_, up_));
    }
    
}

void CameraObject::setCameraFocus(AbstractObject* camera_focus) {
    modeUpdate(camera_mode_, camera_focus);
    camera_focus_ = camera_focus;
}

AbstractObject* CameraObject::getCameraFocus() {
    return camera_focus_;
}

void CameraObject::withMutext(std::function<void(void)> func) {
    assert(func);
    std::lock_guard<std::mutex> guard{ camera_object_mutex_ };
    func();
}

void CameraObject::modeUpdate(CameraMode mode, AbstractObject* focus) {
    if (glm::all(glm::equal(relative_position_, glm::vec3{ 0.0f }))) {
        if (glm::all(glm::equal(position_, glm::vec3{ 0.0f }))) {
            relative_position_ = glm::vec3{ 0.0f, 2.0f, 0.0f } * glm::compMin(camera_focus_->getScale());
        } else {
            relative_position_ = position_;
        }
    }
    switch (mode) {
    case CameraMode::Free:
        speed_ = 1.0f;
        break;
    case CameraMode::Center:
        speed_ = 0.5f * glm::compMin(focus->getScale());
        relative_position_ = (relative_position_ / glm::compMin(camera_focus_->getScale())) * glm::compMin(focus->getScale());
        break;
    case CameraMode::Orbital:
        speed_ = 0.1f * glm::compMin(camera_focus_->getScale());
        relative_position_ = (relative_position_ / glm::compMin(camera_focus_->getScale())) * glm::compMin(focus->getScale());
        break;
    case CameraMode::Rotational:
        speed_ = 0.05f * glm::compMin(focus->getScale());
        relative_position_ = (relative_position_ / glm::compMin(camera_focus_->getScale())) * glm::compMin(focus->getScale());
        break;
    default:
        break;
    }
}


} // namespace object
} // namespace simulation
} // namespace wanderers
