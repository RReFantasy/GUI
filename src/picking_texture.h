#pragma once

#include "glad/glad.h"

class PickingTexture
{
public:
	PickingTexture() {}

	~PickingTexture() {}

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	// called at the start of the picking phase. 
	// Before we start rendering into the picking texture we need to enable it for writing. 
	// This means binding the FBO to the GL_DRAW_FRAMEBUFFER.
	void EnableWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	}

	// called at the end of the picking phase to go back to the default framebuffer.
	// After we finish rendering into the picking texture we tell OpenGL that 
	// from now on we want to render into the default framebuffer 
	// by binding zero to the GL_DRAW_FRAMEBUFFER target.
	void DisableWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	struct PixelInfo
	{
		float ObjectID = 0.0;
		float DrawID = 0.0;
		float PrimID = 0.0;
	};

	/**
	 * To read back the index of a pixel with its screen space coordinate. 
	 * This function returns a structure with the three indices (or IDs) 
	 * If the mouse click didn't touch any object at all the PrimID field of 
	 * the PixelInfo structure will contain 0xFFFFFFFF.
	 */
	PixelInfo ReadPixel(unsigned int x, unsigned int y);

private:
	unsigned int m_fbo;
	unsigned int m_pickingTexture;
	unsigned int m_depthTexture;
};
