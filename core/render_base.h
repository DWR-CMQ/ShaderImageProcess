#pragma once

#include <iostream>
#include <vector>
#include "../defs.h"

class RenderBase
{
public:
	virtual void Init() = 0;
	virtual void Update(psOptions opt) = 0;
	virtual void Render() = 0;
};