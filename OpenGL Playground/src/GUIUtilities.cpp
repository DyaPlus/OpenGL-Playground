#include "GUIUtilities.h"


bool GUI_UT::InputFloat3(std::string name, float* data)
{
    ImGui::TextColored(ImVec4(0.4f, 0.4f, 1.0f, 1.0f), name.c_str());
    ImGui::SameLine();
    if (ImGui::InputFloat3("", data, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        return true;
    return false;
}
