/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * This class represents the frame buffer for rendering.                     *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WANDERERS_RENDER_FRAME_BUFFER_H_
#define WANDERERS_RENDER_FRAME_BUFFER_H_

/* External Includes */

/* Internal Includes */
#include "simulation/object/model/frame.h"

/* STL Includes */

namespace wanderers {
namespace render {

/*
 * Class for the FrameBuffer.
 */
class FrameBuffer {
public:
	FrameBuffer(unsigned int width, unsigned int height);

	~FrameBuffer();

	void bind();
	void unbind();

	void bindTexture();
	void unbindTexture();

protected:
	unsigned int fbo_;
	unsigned int rbo_;

	unsigned int output_texture_;
	unsigned int strength_texture_;
};

} // namespace render
} // namespace wanderers

#endif // WANDERERS_RENDER_FRAME_BUFFER_H_
