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
                          : speed_{1.0f}, position_{ position }, relative_position_{0.0f},
                            orientation_{direction, up},
                            relative_orientation_{ kDefaultDirection, kDefaultUpVector },
                            camera_mode_{ kDefaultCameraMode }, camera_focus_{ nullptr } {}

glm::vec3 CameraObject::getPosition() { 
    return position_; 
}
glm::vec3 CameraObject::getDirection() { 
    return orientation_.getNormal();
}
glm::vec3 CameraObject::getUp() { 
    return orientation_.getTangent(); 
}
glm::vec3 CameraObject::getRight() { 
    return orientation_.getBitangent(); 
}


void CameraObject::setPosition(glm::vec3 position) {
    position_ = position;
}

void CameraObject::setDirection(glm::vec3 direction) {
    orientation_.setNormal(direction);
}

void CameraObject::setUp(glm::vec3 up) {
    orientation_.setTangent(up);
}

void CameraObject::setRight(glm::vec3 right) {
    orientation_.setBitangent(right);
}

void CameraObject::setRelativeDirection(glm::vec3 relative_direction) {
    relative_orientation_.setNormal(relative_direction);
}

/*
 * CameraObject move:
 * - Increment the position with the movement in z direction parallel with the direction vector.
 * - Increment the position with the movement in x direction orthogonal to the direction and y vector.
 * - Increment the position with the movement in y direction orthogonal to the direction and cameras up direction.
 */
void CameraObject::move(glm::vec3 movement) {
    if (camera_mode_ == CameraMode::Center) {
        relative_position_ += relative_orientation_.focusRotation(glm::vec2{ movement }, relative_position_)
                           +  relative_orientation_.translate(glm::vec3{0.0f, 0.0f, movement.z} * speed_ * glm::length(relative_position_));

    } else if (camera_mode_ == CameraMode::Orbital) {
        relative_position_ += relative_orientation_.translate(movement * speed_ * glm::length(relative_position_));
    } else if (camera_mode_ == CameraMode::Rotational) {
        relative_position_ += relative_orientation_.translate(movement * speed_ * glm::length(relative_position_));
    } else {
        position_ += orientation_.translate(movement * speed_);
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
    if (camera_mode_ == CameraMode::Free) {
        orientation_.tangentRotation(glm::radians(yaw));
    }
    else if (camera_mode_ == CameraMode::Center) {
        relative_orientation_.tangentRotation(glm::radians(yaw));
    }
    else if (camera_mode_ == CameraMode::Orbital) {
        relative_orientation_.tangentRotation(glm::radians(yaw));
    }
    else if (camera_mode_ == CameraMode::Rotational) {
        relative_orientation_.tangentRotation(glm::radians(yaw));
    }
    
}

/*
 * CameraObject turnPitch:
 * - Increments the pitch in degrees but don't let it go below -89.0 or above 89.0 .
 */
void CameraObject::turnPitch(float pitch) {
    if (camera_mode_ == CameraMode::Free) {
        orientation_.bitangentRotation(glm::radians(pitch));
    }
    else if (camera_mode_ == CameraMode::Center) {
        relative_orientation_.bitangentRotation(glm::radians(pitch));
    }
    else if (camera_mode_ == CameraMode::Orbital) {
        relative_orientation_.bitangentRotation(glm::radians(pitch));
    } 
    else if (camera_mode_ == CameraMode::Rotational) {
        relative_orientation_.bitangentRotation(glm::radians(pitch));
    }
}

/* CameraObject turnRoll:
 *  - Increments the roll in degrees but keep it in the interval of -360.0 - 360.0 .
 */
void CameraObject::turnRoll(float roll) {
    if (camera_mode_ == CameraMode::Free) {
        orientation_.normalRotation(glm::radians(roll));
    }
    else if (camera_mode_ == CameraMode::Center) {
        relative_orientation_.normalRotation(glm::radians(roll));
    }
    else if (camera_mode_ == CameraMode::Orbital) {
        relative_orientation_.normalRotation(glm::radians(roll));
    }
    else if (camera_mode_ == CameraMode::Rotational) {
        relative_orientation_.normalRotation(glm::radians(roll));
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

glm::mat4 getFocusMatrix(AstronomicalObject* focus) {
    AstronomicalObject* parent{ focus->getParent() };
    glm::mat4 matrix{ 1.0f };
    while (parent != nullptr) {
        matrix = parent->getMatrix() * matrix;
        parent = parent->getParent();
    }
    return matrix;
}

void CameraObject::elapseTime(double seconds) {
    if (camera_mode_ == CameraMode::Free) {
        
    }
    else if (camera_mode_ == CameraMode::Center) {
        glm::mat4 world_matrix{ getFocusMatrix(camera_focus_) * camera_focus_->getPhysicalMatrix() };


        position_ = world_matrix * glm::vec4{ relative_position_, 1.0f };

        orientation_.transform(world_matrix, relative_orientation_);
    } 
    else if (camera_mode_ == CameraMode::Orbital){
        glm::mat4 world_matrix{ getFocusMatrix(camera_focus_) * camera_focus_->getPhysicalMatrix() };

        position_ = world_matrix * glm::vec4{ relative_position_, 1.0f };

        orientation_.transform(world_matrix, relative_orientation_);
    } 
    else if (camera_mode_ == CameraMode::Rotational) {
        glm::mat4 world_matrix{ getFocusMatrix(camera_focus_) * camera_focus_->getMatrix() };

        position_ = world_matrix * glm::vec4{relative_position_, 1.0f};

        orientation_.transform(getFocusMatrix(camera_focus_) * camera_focus_->getRotationalMatrix(), relative_orientation_);

    }
    
}

void CameraObject::setCameraFocus(AstronomicalObject* camera_focus) {
    modeUpdate(camera_mode_, camera_focus);
    camera_focus_ = camera_focus;
}

AstronomicalObject* CameraObject::getCameraFocus() {
    return camera_focus_;
}

void CameraObject::withMutext(std::function<void(void)> func) {
    assert(func);
    std::lock_guard<std::mutex> guard{ camera_object_mutex_ };
    func();
}

void CameraObject::focus() {
    if (camera_mode_ == CameraMode::Free) {
        glm::mat4 world_matrix{ getFocusMatrix(camera_focus_) * camera_focus_->getPhysicalMatrix() };

        setDirection(glm::vec3{ world_matrix * glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f } } - position_);
    }
    else {
        setRelativeDirection(-relative_position_);
    }
}

void CameraObject::modeUpdate(CameraMode mode, AstronomicalObject* focus) {
    if (glm::all(glm::equal(relative_position_, glm::vec3{ 0.0f }))) {
        if (glm::all(glm::equal(position_, glm::vec3{ 0.0f }))) {
            relative_position_ = glm::vec3{ 0.0f, 2.0f, 0.0f } * glm::compMin(camera_focus_->getScale());
        } else {
            relative_position_ = position_;
        }
    }

    if (mode == CameraMode::Free) {
        speed_ = 10.0f;
    }
    else if (mode == CameraMode::Center) {
        speed_ = 0.5f;

        glm::mat4 world_matrix{ getFocusMatrix(camera_focus_) * camera_focus_->getPhysicalMatrix() };

        relative_position_  = glm::vec3{ glm::inverse(world_matrix) * glm::vec4{ position_, 1.0f } };

        relative_orientation_.transform(glm::inverse(world_matrix), orientation_);
    }
    else if (mode == CameraMode::Orbital) {
        speed_ = 0.25f;

        glm::mat4 world_matrix{ getFocusMatrix(camera_focus_) * camera_focus_->getPhysicalMatrix() };

        relative_position_  = glm::vec3{ glm::inverse(world_matrix) * glm::vec4{ position_, 1.0f } };

        relative_orientation_.transform(glm::inverse(world_matrix), orientation_);
    }
    else if (mode == CameraMode::Rotational) {
        speed_ = 0.1f;
        
        glm::mat4 world_matrix{getFocusMatrix(camera_focus_) * camera_focus_->getMatrix() };

        relative_position_  = glm::vec3{ glm::inverse(world_matrix) * glm::vec4{ position_, 1.0f } };

        relative_orientation_.transform(glm::inverse(world_matrix), orientation_);
    }
}


} // namespace object
} // namespace simulation
} // namespace wanderers
