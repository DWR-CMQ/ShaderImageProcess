#pragma once
#include <random>
#define M_PI 3.14159265358979323846

const int kiWidth = 1200;
const int kiHeigth = 800;

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