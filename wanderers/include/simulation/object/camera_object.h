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

#include <mutex>

namespace wanderers {
namespace simulation {
namespace object {

/*
 * Class for the camera object in the simulation.
 */
class CameraObject {
public:
	CameraObject();

	CameraObject(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::vec3 getUp();
	glm::vec3 getRight();

	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	void setUp(glm::vec3 up);
	void setRight(glm::vec3 right);

	/* Translates the camera relative to it's position. */
	void translate(glm::vec3 movement);

	/* Moves the camera relative to it's position and direction. */
	void move(glm::vec3 movement);

	/* Changes the direction of the camera by rotating around the y axis. */
	void turnYaw(float yaw); 
	/* Changes the direction of the camera by rotating around the x axis. */
	void turnPitch(float pitch);
	/* Changes the direction of the camera by rotating around the z axis. */
	void turnRoll(float roll);

protected:
	/* The position of the camera in world space. */
	glm::vec3 position_;

	/* Direction of the camera as a vector. */
	glm::vec3 direction_;
	glm::vec3 right_;
	glm::vec3 up_;

	std::mutex camera_object_mutex_;

private:
	static constexpr glm::vec3 kDefaultPosition{ 0.0f, 0.0f, 0.0f };
	static constexpr glm::vec3 kDefaultDirection{ 0.0f, 0.0f, -1.0f };
	static constexpr glm::vec3 kDefaultUpVector{ 0.0f, 1.0f, 0.0f };
};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_CAMERA_OBJECT_H_
