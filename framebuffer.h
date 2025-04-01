#pragma once
#include <glad/glad.h>
#include <iostream>
class FrameBuffer
{
public:
	GLuint CreateFramebuffer(GLuint& texture, int width, int height);

};

