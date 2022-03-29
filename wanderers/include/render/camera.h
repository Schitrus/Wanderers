/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class represents the camera.                                         *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_RENDER_CAMERA_H_
#define WANDERERS_RENDER_CAMERA_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "render/camera_view.h"
#include "simulation/object/camera_object.h"

namespace wanderers {
namespace render {

/*
 * Class for the Camera.
 */
class Camera : public simulation::object::CameraObject, public CameraView {
public:
	Camera();

	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float field_of_view, float aspect_ratio, float near, float far);

	/* Returns matrix describing the cameras position and direction in world space. */
	glm::mat4 getViewMatrix();

	/* Returns matrix describing the projection of the camera. NOTE: perspective projection. */
	glm::mat4 getProjectionMatrix();

	/* Disable camera from changing. */
	void lock();

	/* Enables camera from changing. */
	void unlock();
protected:

	glm::mat4 view_;
	glm::mat4 projection_;
};

} // namespace render
} // namespace wanderers

#endif // WANDERERS_RENDER_CAMERA_H_
