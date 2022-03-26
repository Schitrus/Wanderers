/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class controls the view and object of the camera.                    *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_CONTROL_CAMERA_CONTROL_H_
#define WANDERERS_CONTROL_CAMERA_CONTROL_H_

/* Internal Includes */
#include "simulation/object/camera_object.h"
#include "render/camera_view.h"

namespace wanderers {
namespace control {

/*
 * Class for the view of the camera and translating it's state into matrices.
 */
class CameraControl {
public:
	CameraControl(simulation::object::CameraObject* camera_object, render::CameraView* camera_view);

	enum class Mode {
		Free, Orbital, Rotational, Count
	};

	Mode getMode();

	void setMode(Mode mode);

	Mode cycleMode();

	unsigned int getFocusId();

	void setFocusId(unsigned int focus_id);

private:
	simulation::object::CameraObject* camera_object_;
	render::CameraView* camera_view_;

	Mode mode_;
	unsigned int focus_id_;

	static constexpr Mode kDefaultMode{ Mode::Free };
};

} // namespace control
} // namespace wanderers

#endif // WANDERERS_CONTROL_CAMERA_CONTROL_H_
