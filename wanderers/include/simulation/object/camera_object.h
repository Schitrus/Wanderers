/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class represents the Camera as an object.                            *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_CAMERA_OBJECT_H_
#define WANDERERS_SIMULATION_OBJECT_CAMERA_OBJECT_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "common/orientation.h"
#include "simulation/object/astronomical_object.h"

/* STL Includes */
#include <mutex>
#include <functional>

namespace wanderers {
namespace simulation {
namespace object {

/*
 * Class for the camera object in the simulation.
 */
class CameraObject {
public:
	/* Mode for how the camera will interact with the simulation. */
	enum class CameraMode {
		Free, Center, Orbital, Rotational, Count
	};

	CameraObject();

	CameraObject(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::vec3 getUp();
	glm::vec3 getRight();

	glm::vec3 getRelativePosition();

	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	void setUp(glm::vec3 up);
	void setRight(glm::vec3 right);

	void setRelativeDirection(glm::vec3 relative_direction);

	/* Translates the camera relative to it's position. */
	void translate(glm::vec3 movement);

	/* Moves the camera relative to it's position and direction. */
	void move(glm::vec3 movement);

	/* Changes the direction of the camera by rotating around the cameras up vector. */
	void turnYaw(float yaw); 
	/* Changes the direction of the camera by rotating around the cameras right vector. */
	void turnPitch(float pitch);
	/* Changes the direction of the camera by rotating around the cameras direction vector. */
	void turnRoll(float roll);

	CameraMode getCameraMode();

	void setCameraMode(CameraMode mode);

	CameraMode cycleCameraMode();

	void elapseTime(double seconds);

	void setCameraFocus(AstronomicalObject* camera_focus);

	AstronomicalObject* getCameraFocus();

	void focus();

	void withMutext(std::function<void(void)> func);

protected:
	/* Updates the camera mode and camera focus. */
	void modeUpdate(CameraMode mode, AstronomicalObject* focus);

	/* The focus of the camera */
	AstronomicalObject* camera_focus_;
	
	CameraMode camera_mode_;

	/* The movement speed of the camera. */
	float speed_;

	/* The position of the camera in world space. */
	glm::vec3 position_;

	/* The position of the camera relative to the focus object. */
	glm::vec3 relative_position_;

	/* Direction of the camera as a vector in world space. */
	common::Orientation orientation_;

	/* Direction of the camera as a vector relative to the focus object. */
	common::Orientation relative_orientation_;

	std::mutex camera_object_mutex_;

private:
	static constexpr glm::vec3 kDefaultPosition{ 0.0f, 0.0f, 0.0f };
	static constexpr glm::vec3 kDefaultDirection{ 0.0f, 0.0f, 1.0f };
	static constexpr glm::vec3 kDefaultUpVector{ 0.0f, 1.0f, 0.0f };

	static constexpr CameraMode kDefaultCameraMode{ CameraMode::Free };
};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_CAMERA_OBJECT_H_
