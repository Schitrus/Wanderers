#ifndef WANDERERS_SIMULATION_OBJECT_ORBIT_H_
#define WANDERERS_SIMULATION_OBJECT_ORBIT_H_

/* Internal Includes */
#include "simulation/object/astronomical_object.h"

class Orbit {
public:
	Orbit(AstronomicalObject* object);

	Orbit(AstronomicalObject* object, float radius, float angular_velocity, glm::vec3 orbital_axis, float orbital_angle);

	~Orbit();

	AstronomicalObject* getOrbitor();

	glm::mat4 getOrbitMatrix();

	void elapseTime(double seconds);

private:
	AstronomicalObject* orbitor_;

	float radius_;

	float angular_velocity_;
	glm::vec3 orbital_axis_;
	float orbital_angle_;

	static constexpr float kDefaultRadius{ 4.0f };
	static constexpr float kDefaultAngularVelocity{ 10.0f };
	static constexpr glm::vec3 kDefaultOrbitalAxis{0.0f, 1.0f, 0.0f};
	static constexpr float kDefaultStartingOrbitalAngle{ 0.0f };
};

#endif // WANDERERS_SIMULATION_OBJECT_ORBIT_H_