#include "ui.h"
#include "common_helper.h"

void UI::SetupUI(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	
}

void UI::RenderUI(psOptions& opts)
{
	ImGui::SetNextWindowSize(ImVec2(400, 100));
	ImGui::Begin("PS");

    if (ImGui::CollapsingHeader("SmartBlur", ImGuiTreeNodeFlags_DefaultOpen))
    {
		CommonHelper::imguiIntSlider("Radius", &opts.iRadius, 1, 19, nullptr, Scale::LOG);
		CommonHelper::imguiFloatSlider("Threshold", &opts.fThreshold, 0.0f, 255.0f, nullptr, Scale::LINEAR);
    }

	ImGui::End();
	ImGui::Render();
}
