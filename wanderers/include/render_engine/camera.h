#ifndef WANDERERS_RENDER_ENGINE_CAMERA_H_
#define WANDERERS_RENDER_ENGINE_CAMERA_H_

/* External Includes */
#include "glm/glm.hpp"

class Camera {
public:
	Camera();

	Camera(glm::vec3 position, float yaw, float pitch, float roll);

	glm::vec3 getPosition();

	glm::mat4 getViewMatrix();

	glm::mat4 getProjectionMatrix();

	void move(glm::vec3 movement);

	void turnYaw(float angle);
	void turnPitch(float angle);
	void turnRoll(float angle);

	void setPosition(glm::vec3 position);

	void setAspectRatio(float aspect_ratio);
	void setAspectRatio(int width, int height);

private:
	glm::vec3 position_;

	float yaw_, pitch_, roll_;
	glm::vec3 direction_;

	float field_of_view_;
	float aspect_ratio_;
	float near_;
	float far_;

	glm::mat4 view_;
	glm::mat4 projection_;

	static constexpr glm::vec3 kDefaultStartingPosition{ 0.0f, 0.0f, 0.0f};
	static constexpr float kDefaultStartingYaw{ 0.0f };
	static constexpr float kDefaultStartingPitch{ 0.0f };
	static constexpr float kDefaultStartingRoll{ 0.0f };
	static constexpr float kDefaultFieldOfView{ 60.0f };
	static constexpr float kDefaultAspectRatio{ 1.0f };
	static constexpr float kDefaultNear{ 0.01f };
	static constexpr float kDefaultFar{ 100.0f };

	static constexpr glm::vec3 kUpVector{ 0.0f, 1.0f, 0.0f };

	glm::vec3 toDirection(float yaw, float pitch);
};

#endif // WANDERERS_RENDER_ENGINE_CAMERA_H_
