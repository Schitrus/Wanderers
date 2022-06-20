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
 * Class for representing orientation in space. 
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

	/* Sets the orientation with the matrix over the passed orientation. */
	void transform(glm::mat4 matrix, Orientation orientation);
	
	/* Returns the movement translated according to the orientation. */
	glm::vec3 translate(glm::vec3 movement);

	/* Rotate the orientation and the passed vector around a point in origo. */
	glm::vec3 focusRotation(glm::vec2 angles_in_radians, glm::vec3 vector);

	/* Rotate around normal/tangent/bitangent and return the passed vector rotated. */
	glm::vec3 normalRotation(float angle_in_radians, glm::vec3 vector = kOrigo);
	glm::vec3 tangentRotation(float angle_in_radians, glm::vec3 vector = kOrigo);
	glm::vec3 bitangentRotation(float angle_in_radians, glm::vec3 vector = kOrigo);

	/* Set normal while keeping tangent and bitangent orthogonal. */
	void setNormal(glm::vec3 normal);
	glm::vec3 getNormal() const;

	/* Set tangent while keeping bitangent and normal orthogonal. */
	void setTangent(glm::vec3 tangent);
	glm::vec3 getTangent() const;

	/* Set bitangent while keeping normal and tangent orthogonal. */
	void setBitangent(glm::vec3 bitangent);
	glm::vec3 getBitangent() const;

	/* Get the matrix for the orientation from the default orientation. */
	glm::mat4 orientationMatrix();
	/* Get the matrix for the orientation from the passed orientation. */
	glm::mat4 orientationMatrix(Orientation from);

protected:
	/* Normalized a vector, asserts error if vector is zero. */
	glm::vec3 normalize(glm::vec3 vector);

	/* Returns a vector that is orthogonal to the normal based on the vector given. */
	glm::vec3 orthogonalize(glm::vec3 vector, glm::vec3 normal);

	/* Sets the passed normal to axis, and sets passed tangent and bitangent correspondingly. */
	void setAxis(glm::vec3 axis, glm::vec3& normal, glm::vec3& tangent, glm::vec3& bitangent);

	/* Rotate tangent and bitangent around the normal and return the passed vector rotated. */
	glm::vec3 rotation(float angle_in_radians, glm::vec3 vector, glm::vec3 normal, glm::vec3& tangent, glm::vec3& bitangent);

	glm::vec3 normal_;
	glm::vec3 tangent_;
	glm::vec3 bitangent_;
};

static const Orientation kXOrientation{Orientation::kRight, Orientation::kUp};
static const Orientation kYOrientation{Orientation::kUp, Orientation::kFront};
static const Orientation kZOrientation{Orientation::kFront, Orientation::kUp};

} // namespace common
} // namespace wanderers

#endif // WANDERERS_COMMON_ORIENTATION_H_
