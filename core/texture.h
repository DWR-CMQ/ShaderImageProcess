#pragma once
#include <glad/glad.h>
#include <iostream>
class Texture
{
public:

	unsigned int loadTexture(char const* path, int& width, int& height);
	unsigned int loadTexture(char const* path);
};

