#pragma once
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "defs.h"
class UI
{
public:
	static void SetupUI(GLFWwindow* window);
	static void RenderUI(SmartBlur& opts);
};

