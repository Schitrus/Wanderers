/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                           *
 * Implementation of the FrameBuffer class.                                   *
 *                                                                           *
 * Copyright (c) 2022 Karl Andersson                                         *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "render/frame_buffer.h"

/* External Includes */
#include <glad/gl.h>

/* STL Includes */
#include <iostream>

namespace wanderers {
namespace render {

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) : fbo_{ 0 } {
	glGenFramebuffers(1, &fbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	// color buffer
	glGenTextures(1, &output_texture_);
	glBindTexture(GL_TEXTURE_2D, output_texture_);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, output_texture_, 0);

	// color strength
	glGenTextures(1, &strength_texture_);
	glBindTexture(GL_TEXTURE_2D, strength_texture_);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, strength_texture_, 0);

	// depth buffer
	glGenRenderbuffers(1, &rbo_);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Error generating FrameBuffer" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, output_texture_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, strength_texture_);
}

void FrameBuffer::unbindTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &fbo_);
}

} // namespace render
} // namespace wanderers
