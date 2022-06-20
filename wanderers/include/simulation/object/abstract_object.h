/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Abstract class for representation of objects.                             *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_SIMULATION_OBJECT_ABSTRACT_OBJECT_H_
#define WANDERERS_SIMULATION_OBJECT_ABSTRACT_OBJECT_H_

/* External Includes */
#include "glm/glm.hpp"

/* Internal Includes */
#include "common/orientation.h"

namespace wanderers {
namespace simulation {
namespace object {

/*
 * This class is the abstraction for objects. 
 */
class AbstractObject {
public:
	static constexpr glm::vec3 kOrigo{ 0.0f, 0.0f, 0.0f };

	static constexpr glm::vec3 kUp{ 0.0f, 1.0f, 0.0f };
	static constexpr glm::vec3 kFace{ 0.0f, 0.0f, 1.0f };
	static constexpr glm::vec3 kSide{ 1.0f, 0.0f, 0.0f };

	static constexpr glm::vec3 kIdentityScale{ 1.0f, 1.0f, 1.0f };

	AbstractObject();

	AbstractObject(glm::vec3 position, glm::vec3 orientation, glm::vec3 face, glm::vec3 scale, AbstractObject* parent = nullptr);

	AbstractObject(const AbstractObject& abstract_object) = default;

	std::uint64_t getObjectId() const;

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition() const;

	void setOrientation(glm::vec3 orientation);
	glm::vec3 getOrientation() const;

	void setFace(glm::vec3 face);
	glm::vec3 getFace() const;

	void setSide(glm::vec3 side);
	glm::vec3 getSide() const;

	void setScale(glm::vec3 scale);
	glm::vec3 getScale() const;

	virtual glm::mat4 getMatrix();

	virtual void elapseTime(double seconds);

protected:
	glm::vec3 position_;

	common::Orientation orientation_;

	glm::vec3 scale_;

private:
	const std::uint64_t object_id_;

	static std::uint64_t id_counter_;
};

static const AbstractObject kDefaultObject{};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ABSTRACT_OBJECT_H_
