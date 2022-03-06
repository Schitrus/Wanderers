/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class controls the Camera and calculates matrices it uses in render  *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_RENDER_CAMERA_H_
#define WANDERERS_RENDER_CAMERA_H_

/* External Includes */
#include "glm/glm.hpp"

/* STL Includes */
#include <mutex>

// TODO: Add more camera modes.

namespace wanderers {
namespace render {

/*
 * Class for controlling the camera and translating it's state into matrices.
 */
class Camera {
public:
	Camera();

	Camera(glm::vec3 position, float yaw, float pitch, float roll);

	glm::vec3 getPosition();

	/* Returns matrix describing the cameras position and direction in world space. */
	glm::mat4 getViewMatrix();

	/* Returns matrix describing the projection of the camera. NOTE: perspective projection. */
	glm::mat4 getProjectionMatrix();

	/* Moves the camera relative to it's position and direction. */
	void move(glm::vec3 movement);

	/* Changes the direction of the camera by rotating around the y axis. */
	void turnYaw(float angle); 
	/* Changes the direction of the camera by rotating around the x axis. */
	void turnPitch(float angle);
	/* Changes the direction of the camera by rotating around the z axis. 
	   Note: Not used as of the moment. */
	void turnRoll(float angle);

	/* Sets the absolute position of the camera. */
	void setPosition(glm::vec3 position);

	void setAspectRatio(float aspect_ratio);
	void setAspectRatio(int width, int height);

	/* Disable camera from changing. */
	void lock();

	/* Enables camera from changing. */
	void unlock();

private:
	/* The position of the camera in world space. */
	glm::vec3 position_;

	/* The direction of the camera described in angle degrees.*/
	float yaw_, pitch_, roll_;
	/* Direction of the camera as a vector. */
	glm::vec3 direction_;

	float field_of_view_;
	float aspect_ratio_;
	/* Describes the plane closest to the camera where objects are clipped. */
	float near_;
	/* Describes the plane furthest away from the camera where objects are clipped. */
	float far_;

	glm::mat4 view_;
	glm::mat4 projection_;

	std::mutex camera_mutex_;

	static constexpr glm::vec3 kDefaultStartingPosition{ 0.0f, 0.0f, 0.0f};
	static constexpr float kDefaultStartingYaw{ 0.0f };
	static constexpr float kDefaultStartingPitch{ 0.0f };
	static constexpr float kDefaultStartingRoll{ 0.0f };
	static constexpr float kDefaultFieldOfView{ 60.0f };
	static constexpr float kDefaultAspectRatio{ 1.0f };
	static constexpr float kDefaultNear{ 0.01f };
	static constexpr float kDefaultFar{ 1000.0f };

	static constexpr glm::vec3 kUpVector{ 0.0f, 1.0f, 0.0f };

	/* Translates the direction angles to direction vector. */
	glm::vec3 toDirection(float yaw, float pitch);
};

} // namespace render
} // namespace wanderers

#endif // WANDERERS_RENDER_CAMERA_H_
