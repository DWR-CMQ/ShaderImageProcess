#include "ui.h"

void UI::SetupUI(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void UI::RenderUI()
{
	ImGui::Begin("Model Render");

    if (ImGui::CollapsingHeader("SmartBlur", ImGuiTreeNodeFlags_DefaultOpen))
    {

    }


	ImGui::End();
	ImGui::Render();
}
