#include "hud.h"
#include "../../../external/imgui/imgui.h"
#include "../config/config.h"
#include "../hooks/hooks.h"
#include <ctime>
#include <string>
#include <sstream>
#include <DirectXMath.h>
#include "hudstyle.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


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
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    float padding = 5.0f;

    // Initialize keybindPos if not set (to center vertically on first render)
    static bool keybindPosInitialized = false;
    if (!keybindPosInitialized && Config::show_keybinds) {
        Config::keybindPos = ImVec2(10.0f, ImGui::GetIO().DisplaySize.y / 2);
        keybindPosInitialized = true;
    }

    // Static variables for dragging state
    static bool isDraggingWatermark = false;
    static bool isDraggingKeybinds = false;

    // Watermark
    if (Config::show_watermark) {
        std::string watermarkText = "Mozority | Beta Test";
        ImVec2 textSize = ImGui::CalcTextSize(watermarkText.c_str());
        ImVec2 rectSize = ImVec2(textSize.x + padding * 2, textSize.y + padding * 2 + 6.0f); // Extra height for gradient bar
        ImVec2 pos = Config::watermarkPos;

        // Menu-style rendering
        ImVec2 mainRectMin = pos;
        ImVec2 mainRectMax = ImVec2(pos.x + rectSize.x, pos.y + rectSize.y);
        ImVec2 gradientRectMin = pos;
        ImVec2 gradientRectMax = ImVec2(pos.x + rectSize.x, pos.y + 6.0f);

        drawList->AddRectFilled(mainRectMin, mainRectMax, mmenu_groupbox_color, 3.0f);
        drawList->AddRectFilledMultiColor(gradientRectMin, gradientRectMax, mmenu_gradient_left_color, mmenu_gradient_right_color, mmenu_gradient_right_color, mmenu_gradient_left_color);
        drawList->AddText(segoe_ui_semibold, 14.0f, ImVec2(pos.x + padding, pos.y + padding + 6.0f), mmenu_first_text_color, watermarkText.c_str());

        // Dragging functionality
        if (ImGui::IsMouseHoveringRect(mainRectMin, mainRectMax)) {
            if (ImGui::IsMouseClicked(0)) {
                isDraggingWatermark = true;
            }
        }
        if (isDraggingWatermark && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            Config::watermarkPos.x += delta.x;
            Config::watermarkPos.y += delta.y;

            // Clamp to screen bounds
            Config::watermarkPos.x = std::max(0.0f, std::min(Config::watermarkPos.x, ImGui::GetIO().DisplaySize.x - rectSize.x));
            Config::watermarkPos.y = std::max(0.0f, std::min(Config::watermarkPos.y, ImGui::GetIO().DisplaySize.y - rectSize.y));
        }
        if (ImGui::IsMouseReleased(0)) {
            isDraggingWatermark = false;
        }
    }

    // Keybind List
    if (Config::show_keybinds) {
        std::string keybindText = "Keybinds\n";
        keybindText += "Aimbot: " + std::string(Config::aimbot ? "ON" : "OFF");

        ImVec2 textSize = ImGui::CalcTextSize(keybindText.c_str());
        ImVec2 rectSize = ImVec2(textSize.x + padding * 2, textSize.y + padding * 2 + 6.0f); // Extra height for gradient bar
        ImVec2 pos = Config::keybindPos;

        // Menu-style rendering
        ImVec2 mainRectMin = pos;
        ImVec2 mainRectMax = ImVec2(pos.x + rectSize.x, pos.y + rectSize.y);
        ImVec2 gradientRectMin = pos;
        ImVec2 gradientRectMax = ImVec2(pos.x + rectSize.x, pos.y + 6.0f);

        drawList->AddRectFilled(mainRectMin, mainRectMax, mmenu_groupbox_color, 8.0f); // More rounded corners
        drawList->AddRectFilledMultiColor(gradientRectMin, gradientRectMax, mmenu_gradient_left_color, mmenu_gradient_right_color, mmenu_gradient_right_color, mmenu_gradient_left_color);
        drawList->AddText(segoe_ui_semibold, 12.0f, ImVec2(pos.x + padding, pos.y + padding + 6.0f), mmenu_first_text_color, keybindText.c_str());

        // Dragging functionality
        if (ImGui::IsMouseHoveringRect(mainRectMin, mainRectMax)) {
            if (ImGui::IsMouseClicked(0)) {
                isDraggingKeybinds = true;
            }
        }
        if (isDraggingKeybinds && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            Config::keybindPos.x += delta.x;
            Config::keybindPos.y += delta.y;

            // Clamp to screen bounds
            Config::keybindPos.x = std::max(0.0f, std::min(Config::keybindPos.x, ImGui::GetIO().DisplaySize.x - rectSize.x));
            Config::keybindPos.y = std::max(0.0f, std::min(Config::keybindPos.y, ImGui::GetIO().DisplaySize.y - rectSize.y));
        }
        if (ImGui::IsMouseReleased(0)) {
            isDraggingKeybinds = false;
        }
    }

    // FOV Circle (unchanged)
    if (Config::fov_circle) {
        ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x / 2.f, ImGui::GetIO().DisplaySize.y / 2.f);
        RenderFovCircle(drawList, Config::aimbot_fov, Center, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y, 1.f);
    }
}