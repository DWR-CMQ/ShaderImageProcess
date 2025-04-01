#include <iostream>
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "shader.h"
#include "texture.h"
#include "framebuffer.h"
#include "ui.h"

// 窗口尺寸
const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

// 全屏四边形顶点数据
float quadVertices[] = {
	// 位置 // 纹理坐标
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

int main() 
{
	// 初始化GLFW
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口
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
	
	// 创建输入纹理
	int width, height;
	Texture input;
	unsigned int inputTexture = input.loadTexture("1.jpg", width, height);

	Shader rgbToYCrCbProgram("assets/shader/display.vs", "assets/shader/smartblurbuffer.fs");
	Shader smartBlurProgram("assets/shader/display.vs", "assets/shader/smartblur.fs");
	Shader displayProgram("assets/shader/display.vs", "assets/shader/display.fs");

	// 创建VAO和VBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// 位置属性
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// 纹理坐标属性
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// 创建中间纹理和帧缓冲
	GLuint ycrcbTexture;
	FrameBuffer ycrcbFBO;
	GLuint ycrcb = ycrcbFBO.CreateFramebuffer(ycrcbTexture, width, height);

	GLuint blurredTexture;
	FrameBuffer blurredFBO;
	GLuint blur = blurredFBO.CreateFramebuffer(blurredTexture, width, height);

	ImGuiIO& io = ImGui::GetIO();
	// 主渲染循环
	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		UI::RenderUI();

		// 第一阶段：RGB转YCrCb（渲染到帧缓冲）
		glBindFramebuffer(GL_FRAMEBUFFER, ycrcb);
		glViewport(0, 0, width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(rgbToYCrCbProgram.ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputTexture);
		glUniform1i(glGetUniformLocation(rgbToYCrCbProgram.ID, "inputImage"), 0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 第二阶段：智能模糊（渲染到默认帧缓冲）
		glBindFramebuffer(GL_FRAMEBUFFER, blur);
		glViewport(0, 0, width, height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(smartBlurProgram.ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ycrcbTexture);
		glUniform1i(glGetUniformLocation(smartBlurProgram.ID, "inputImage"), 0);
		glUniform1i(glGetUniformLocation(smartBlurProgram.ID, "radius"), 10);
		glUniform1f(glGetUniformLocation(smartBlurProgram.ID, "threshold"), 24.0f);
		glUniform2f(glGetUniformLocation(smartBlurProgram.ID, "resolution"), (float)width, (float)height);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 第三步：显示结果（分两个视口）
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 左侧视口：原始图像
		glViewport(0, 0, WIDTH / 2, HEIGHT);
		glUseProgram(displayProgram.ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputTexture);
		glUniform1i(glGetUniformLocation(displayProgram.ID, "inputImage"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 右侧视口：处理后的图像
		glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
		glUseProgram(displayProgram.ID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blurredTexture);
		glUniform1i(glGetUniformLocation(displayProgram.ID, "inputImage"), 0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// 清理资源
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &inputTexture);
	glDeleteTextures(1, &ycrcbTexture);
	glDeleteTextures(1, &blurredTexture);
	glDeleteFramebuffers(1, &ycrcb);
	glDeleteFramebuffers(1, &blur);

	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
