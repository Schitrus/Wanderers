/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class represents the camera as a view.                               *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_RENDER_CAMERA_VIEW_H_
#define WANDERERS_RENDER_CAMERA_VIEW_H_

/* External Includes */
#include "glm/glm.hpp"

namespace wanderers {
namespace render {

/*
 * Class for the view of the camera and translating it's state into matrices.
 */
class CameraView {
public:
	CameraView();

	CameraView(float field_of_view, float aspect_ratio, float near, float far);

	/* Returns matrix describing the cameras position and direction in world space. */
	glm::mat4 getViewMatrix();

	/* Returns matrix describing the projection of the camera. NOTE: perspective projection. */
	glm::mat4 getProjectionMatrix();

	float getFieldOfView();
	float getAspectRatio();
	float getNear();
	float getFar();

	void setFieldOfView(float field_of_view);
	void setAspectRatio(float aspect_ratio);
	void setAspectRatio(int width, int height);
	void setNear(float near);
	void setFar(float far);

private:

	float field_of_view_;
	float aspect_ratio_;

	/* Describes the plane closest to the camera where objects are clipped. */
	float near_;
	/* Describes the plane furthest away from the camera where objects are clipped. */
	float far_;

	glm::mat4 view_;
	glm::mat4 projection_;

	static constexpr float kDefaultFieldOfView{ 60.0f };
	static constexpr float kDefaultAspectRatio{ 1.0f };
	static constexpr float kDefaultNear{ 0.01f };
	static constexpr float kDefaultFar{ 1000.0f };
};

} // namespace render
} // namespace wanderers

#endif // WANDERERS_RENDER_CAMERA_VIEW_H_
