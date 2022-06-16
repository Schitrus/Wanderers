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

namespace wanderers {
namespace simulation {
namespace object {

// MAKE ORIENTATION, FACE, SIDE ITS OWN CLASS CLASS

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

	void setParent(AbstractObject* parent);
	AbstractObject* getParent() const;

	virtual glm::mat4 getMatrix();

	virtual void elapseTime(double seconds);

protected:
	glm::vec3 orthogonalize(glm::vec3 v1, glm::vec3 v2);

	glm::vec3 position_;

	glm::vec3 orientation_;
	glm::vec3 face_;
	glm::vec3 side_;

	glm::vec3 scale_;

	AbstractObject* parent_;

private:
	const std::uint64_t object_id_;

	static std::uint64_t id_counter_;
};

static const AbstractObject kDefaultObject{};

} // namespace object
} // namespace simulation
} // namespace wanderers

#endif // WANDERERS_SIMULATION_OBJECT_ABSTRACT_OBJECT_H_
