#include "opengl_window.h"
#include <iostream>


OpenGLWindow::OpenGLWindow()
{
}

bool OpenGLWindow::CreateOpenglWindow(const std::string& windowTitle, int majorVersion, int minorVersion, bool showFullScreen)
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow = glfwCreateWindow(kiWidth, kiHeight, "PS", NULL, NULL);
	if (m_pWindow == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_pWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

GLFWwindow* OpenGLWindow::GetWindow() const
{
	return m_pWindow;
}

void OpenGLWindow::Run()
{
	InitializeScene();
	UI::SetupUI(GetWindow());
	ImGuiIO& io = ImGui::GetIO();

	while (glfwWindowShouldClose(m_pWindow) == 0)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderScene();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(m_pWindow);

		UpdateScene();
	}

	ReleaseScene();
	glfwDestroyWindow(m_pWindow);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}


void OpenGLWindow::CloseWindow()
{
	glfwSetWindowShouldClose(m_pWindow, true);
}