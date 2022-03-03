#ifndef WANDERERS_SIMULATION_OBJECT_PLANET_H_
#define WANDERERS_SIMULATION_OBJECT_PLANET_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "simulation/object/astronomical_object.h"

class Planet : public AstronomicalObject {
public:
	Planet();

	Planet(glm::vec3 surface_color, float radius, 
		   float angular_velocity, glm::vec3 rotational_axis, float rotational_angle);

	glm::vec3 getColor();

	glm::mat4 getPlanetMatrix();

	void elapseTime(double seconds);

private:
	glm::vec3 surface_color_;

	float radius_;

	float angular_velocity_;
	glm::vec3 rotational_axis_;
	float rotational_angle_;

	static constexpr glm::vec3 kDefaultSurfaceColor{ 1.0f, 1.0f, 1.0f };
	static constexpr float kDefaultRadius{ 0.25f };
	static constexpr float kDefaultAngularVelocity{ 60.0f };
	static constexpr glm::vec3 kDefaultRotationalAxis{ 0.0f, 1.0f, 0.0f };
	static constexpr float kDefaultStartingRotationalAngle{ 0.0f };

	friend class SpaceRenderer;
};

#endif // WANDERERS_SIMULATION_OBJECT_PLANET_H_
