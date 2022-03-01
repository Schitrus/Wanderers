#ifndef WANDERERS_SIMULATION_OBJECT_SOLAR_H_
#define WANDERERS_SIMULATION_OBJECT_SOLAR_H_

#include <glm/glm.hpp>

#include "simulation/object/astronomical_object.h"

class Solar : public AstronomicalObject{
public:
	Solar();

	Solar(float temperature, float radius,
		  float angular_velocity, glm::vec3 rotational_axis, float rotational_angle);

	glm::vec3 getColor();

	glm::mat4 getSolarMatrix();

	void elapseTime(double seconds);

private:
	float temperature_;
	
	float radius_;

	float angular_velocity_;
	glm::vec3 rotational_axis_;
	float rotational_angle_;

	static constexpr float kDefaultTemperature{ 2500.0f };
	static constexpr float kDefaultRadius{ 1.0f };
	static constexpr float kDefaultAngularVelocity{ 0.0f };
	static constexpr glm::vec3 kDefaultRotationalAxis{ 0.0f, 1.0f, 0.0f };
	static constexpr float kDefaultStartingRotationalAngle{ 0.0f };

	friend class SpaceRenderer;
};

#endif // WANDERERS_SIMULATION_OBJECT_SOLAR_H_
