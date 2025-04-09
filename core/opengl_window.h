#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include <tuple>
#include <vector>
#include "defs.h"
#include "../ui.h"

class OpenGLWindow
{
public:
	OpenGLWindow();
	virtual ~OpenGLWindow() = default;

	bool CreateOpenglWindow(const std::string& windowTitle, int majorVersion, int minorVersion, bool showFullScreen);
	GLFWwindow* GetWindow() const;
	void Run();

	virtual void InitializeScene(){}
	virtual void UpdateScene(){}
	virtual void RenderScene(){}
	virtual void SaveScene(){}
	virtual void ReleaseScene(){}

	void CloseWindow();

	double GetTimeDelta() const;
	int GetFPS() const;

private:
	GLFWwindow* m_pWindow = nullptr;
	int m_iFps = 0;
	int m_iNextFps = 0;
	int m_iScreenWidth = 0;
	int m_iScreenHeight = 0;
};

