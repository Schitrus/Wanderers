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

	virtual glm::vec3 getPosition();
	virtual glm::vec3 getDirection();
	virtual glm::vec3 getUp();
	virtual glm::vec3 getRight();

	virtual void setPosition(glm::vec3 position);
	virtual void setDirection(glm::vec3 direction);
	virtual void setUp(glm::vec3 up);
	virtual void setRight(glm::vec3 right);

	/* Translates the camera relative to it's position. */
	virtual void translate(glm::vec3 movement);

	/* Moves the camera relative to it's position and direction. */
	virtual void move(glm::vec3 movement);

	/* Changes the direction of the camera by rotating around the y axis. */
	virtual void turnYaw(float yaw); 
	/* Changes the direction of the camera by rotating around the x axis. */
	virtual void turnPitch(float pitch);
	/* Changes the direction of the camera by rotating around the z axis. */
	virtual void turnRoll(float roll);

protected:
	/* The position of the camera in world space. */
	glm::vec3 position_;

	/* Direction of the camera as a vector. */
	glm::vec3 direction_;
	glm::vec3 up_;
	glm::vec3 right_;

private:
	static constexpr glm::vec3 kDefaultPosition{ 0.0f, 0.0f, 0.0f };
	static constexpr glm::vec3 kDefaultDirection{ 0.0f, 0.0f, -1.0f };
	static constexpr glm::vec3 kDefaultUpVector{ 0.0f, 1.0f, 0.0f };
};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_CAMERA_OBJECT_H_
