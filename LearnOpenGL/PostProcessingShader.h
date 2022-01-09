#pragma once

#include "ShaderProgram.h"

class PostProcessingShader
{
public:
	PostProcessingShader(const char* shaderPath);

	void beginRendering();
	void draw();

private:
	void setupFramebuffer();
	void setupScreenQuad();

	ShaderProgram program;
	unsigned int framebuffer;
	unsigned int quadVAO;
	unsigned int textureColorBuffer;
	static const float quadVertices[];
};

