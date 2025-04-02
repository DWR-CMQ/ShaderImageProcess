#include "common_helper.h"


// Helper to display a little (?) mark which shows a tooltip when hovered.
void CommonHelper::imguiHelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

// Helper for a float slider value.
bool CommonHelper::imguiFloatSlider(const char* desc, float* value, float min,
    float max, const char* fmt, Scale scale)
{
    ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    if (scale == Scale::LOG)
    {
        flags = ImGuiSliderFlags_Logarithmic;
    }
    return ImGui::SliderScalar(desc, ImGuiDataType_Float, value, &min, &max, fmt, flags);
}

bool CommonHelper::imguiIntSlider(const char* desc, int* value, int min, int max, const char* fmt, Scale scale)
{
    ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    if (scale == Scale::LOG)
    {
        flags = ImGuiSliderFlags_Logarithmic;
    }
    return ImGui::SliderScalar(desc, ImGuiDataType_S32, value, &min, &max, fmt, flags);
}

//void CommonHelper::PrintMat4(const glm::mat4& mat)
//{
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            std::cout << mat[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
//}
