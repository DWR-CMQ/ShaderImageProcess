#pragma once
#include <random>
#define M_PI 3.14159265358979323846

const int kiWidth = 1200;
const int kiHeight = 800;

enum class Scale
{
	LINEAR = 0,
	LOG
};

struct SmartBlur 
{
	int iRadius = 3;
	float fThreshold = 1.0;
};

// 全屏四边形顶点数据
const float quadVertices[] =
{
	// 位置 // 纹理坐标
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};