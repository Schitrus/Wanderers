/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Class to represent orientation in three dimensional space.                *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_COMMON_ORIENTATION_H_
#define WANDERERS_COMMON_ORIENTATION_H_

/* External Includes */
#include "glm/glm.hpp"

namespace wanderers {
namespace common {


/*
 * This class for representing orientaion. 
 */
class Orientation {
public:
	static constexpr glm::vec3 kOrigo{ 0.0f, 0.0f, 0.0f };

	static constexpr glm::vec3 kFront{ 0.0f, 0.0f, 1.0f };
	static constexpr glm::vec3 kUp{ 0.0f, 1.0f, 0.0f };
	static constexpr glm::vec3 kRight{ 1.0f, 0.0f, 0.0f };

	static constexpr glm::vec3 kIdentityScale{ 1.0f, 1.0f, 1.0f };

	Orientation();

	Orientation(glm::vec3 normal, glm::vec3 tangent);

	void transform(glm::mat4 matrix, Orientation orientation);
	
	glm::vec3 translate(glm::vec3 movement);

	glm::vec3 focusRotation(glm::vec2 angles_in_radians, glm::vec3 point);

	glm::vec3 normalRotation(float angle_in_radians, glm::vec3 point = kOrigo);
	glm::vec3 tangentRotation(float angle_in_radians, glm::vec3 point = kOrigo);
	glm::vec3 bitangentRotation(float angle_in_radians, glm::vec3 point = kOrigo);

	void setNormal(glm::vec3 normal);
	glm::vec3 getNormal();

	void setTangent(glm::vec3 tangent);
	glm::vec3 getTangent();

	void setBitangent(glm::vec3 bitangent);
	glm::vec3 getBitangent();

protected:

	glm::vec3 normalize(glm::vec3 normal);

	glm::vec3 orthogonalize(glm::vec3 v1, glm::vec3 v2);

	glm::vec3 normal_;
	glm::vec3 tangent_;
	glm::vec3 bitangent_;
};

} // namespace common
} // namespace wanderers

#endif // WANDERERS_COMMON_ORIENTATION_H_
