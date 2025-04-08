#include <iostream>
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ui.h"
#include "core/smartblur/ps_smartblur.h"
#include "core/lut/ps_lut.h"

// ���ڳߴ�
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

int main() 
{
	// ��ʼ��GLFW
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "YCrCb Conversion & Smart Blur", NULL, NULL);
	if (!window) 
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	UI::SetupUI(window);

	// ����
	SmartBlur stSmartBlur;
	PSSmartBlur psSB;
	PSLut psLut;

	ImGuiIO& io = ImGui::GetIO();
	// ����Ⱦѭ��
	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		UI::RenderUI(stSmartBlur);
		//psSB.Update(stSmartBlur);
		//psSB.Render();
		psLut.Update(stSmartBlur);
		psLut.Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
