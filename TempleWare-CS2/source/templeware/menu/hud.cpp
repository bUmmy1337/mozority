#include "hud.h"
#include "../../../external/imgui/imgui.h"
#include "../config/config.h"
#include "../hooks/hooks.h"
#include <ctime>
#include <string>
#include <sstream>
#include <DirectXMath.h>

Hud::Hud() {

}

float CalculateFovRadius(float fovDegrees, float screenWidth, float screenHeight, float gameVerticalFOV) {
    float aspectRatio = screenWidth / screenHeight;
    float fovRadians = fovDegrees * (DirectX::XM_PI / 180.0f);

    float screenRadius = std::tan(fovRadians / 2.0f) * (screenHeight / 2.0f) / std::tan(gameVerticalFOV * (DirectX::XM_PI / 180.0f) / 2.0f);

    static float flScalingMultiplier = 2.5f;

    return screenRadius * flScalingMultiplier;
}

void RenderFovCircle(ImDrawList* drawList, float fov, ImVec2 screenCenter, float screenWidth, float screenHeight, float thickness) {
    float radius = CalculateFovRadius(fov, screenWidth, screenHeight, H::g_flActiveFov);
    uint32_t color = ImGui::ColorConvertFloat4ToU32(Config::fovCircleColor);
    drawList->AddCircle(screenCenter, radius, color, 100, thickness);
}

void Hud::render() {
    // Time
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now);
    char timeBuffer[9];
    std::strftime(timeBuffer, sizeof(timeBuffer), "", &localTime);

    // FPS
    float fps = ImGui::GetIO().Framerate;
    std::ostringstream fpsStream;
    fpsStream << static_cast<int>(fps) << "";

    // WaterMark
    std::string watermarkText = "";

    ImVec2 textSize = ImGui::CalcTextSize(watermarkText.c_str());
    float padding = 5.0f;
    ImVec2 pos = ImVec2(10, 10);
    ImVec2 rectSize = ImVec2(textSize.x + padding * 2, textSize.y + padding * 2);

    ImU32 bgColor = IM_COL32(50, 50, 50, 200);
    ImU32 borderColor = IM_COL32(153, 76, 204, 255);
    ImU32 textColor = IM_COL32(255, 255, 255, 255);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    //drawList->AddRectFilled(pos, ImVec2(pos.x + rectSize.x, pos.y + rectSize.y), bgColor);

    //float lineThickness = 2.0f;
    //drawList->AddLine(pos, ImVec2(pos.x, pos.y + rectSize.y), borderColor, lineThickness);
    //drawList->AddLine(ImVec2(pos.x + rectSize.x, pos.y), ImVec2(pos.x + rectSize.x, pos.y + rectSize.y), borderColor, lineThickness);

    ImVec2 textPos = ImVec2(pos.x + padding, pos.y + padding);
    drawList->AddText(textPos, textColor, watermarkText.c_str());

    // Keybind List (Left-Center)
    if (Config::show_keybinds) {
        std::string keybindText = "Keybinds:\n";
        keybindText += "Aimbot: " + std::string(Config::aimbot ? "ON" : "OFF");

        ImVec2 keybindTextSize = ImGui::CalcTextSize(keybindText.c_str());
        ImVec2 keybindPos = ImVec2(10, ImGui::GetIO().DisplaySize.y / 2 - keybindTextSize.y / 2);
        ImVec2 keybindRectSize = ImVec2(keybindTextSize.x + padding * 2, keybindTextSize.y + padding * 2);

        // ÷вета из ApplyImGuiTheme
        ImU32 keybindBgColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.96f, 0.96f, 0.96f, 0.85f)); // panelColor с прозрачностью
        ImU32 keybindBorderColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85f, 0.85f, 0.90f, 1.0f)); // accentColor
        ImU32 keybindTextColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.15f, 0.15f, 0.15f, 1.0f));   // text

        // —кругление и отрисовка
        float rounding = 6.0f;
        drawList->AddRectFilled(keybindPos, ImVec2(keybindPos.x + keybindRectSize.x, keybindPos.y + keybindRectSize.y), keybindBgColor, rounding);
        drawList->AddRect(keybindPos, ImVec2(keybindPos.x + keybindRectSize.x, keybindPos.y + keybindRectSize.y), keybindBorderColor, rounding, 0, 1.0f);

        ImVec2 keybindTextPos = ImVec2(keybindPos.x + padding, keybindPos.y + padding);
        drawList->AddText(keybindTextPos, keybindTextColor, keybindText.c_str());
    }

    if (Config::fov_circle) {
        ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f);
        RenderFovCircle(drawList, Config::aimbot_fov, Center, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, 1.f);
    }
}