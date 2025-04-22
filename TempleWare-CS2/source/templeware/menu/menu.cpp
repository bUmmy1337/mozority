#include "menu.h"
#include "../config/config.h"

#include <iostream>
#include <vector>
#include "../config/configmanager.h"

#include "../keybinds/keybinds.h"

#include "../utils/logging/log.h"



Menu::Menu() {
    activeTab = 0;
    showMenu = true;
    aimSubTab = 0;
    visualsSubTab = 0;
    miscSubTab = 0;
}

static void CustomColorPicker(const char* label, float col[4]) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));

    if (ImGui::ColorButton(label, ImVec4(col[0], col[1], col[2], col[3]),
        ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip)) {
        ImGui::OpenPopup(label);
    }

    if (ImGui::BeginPopup(label)) {
        ImGui::PushItemWidth(200);
        ImGui::ColorPicker4("##picker", col,
            ImGuiColorEditFlags_DisplayRGB |
            ImGuiColorEditFlags_AlphaBar |
            ImGuiColorEditFlags_NoSidePreview |
            ImGuiColorEditFlags_PickerHueBar);
        ImGui::PopItemWidth();
        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(3);
}

void ApplyImGuiTheme() {
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Base colors
    const ImVec4 darkBg = ImVec4(0.11f, 0.11f, 0.12f, 1.00f);
    const ImVec4 mediumBg = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    const ImVec4 lightBg = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
    const ImVec4 accentColor = ImVec4(0.35f, 0.35f, 0.38f, 1.00f);
    const ImVec4 accentHover = ImVec4(0.45f, 0.45f, 0.48f, 1.00f);
    const ImVec4 accentActive = ImVec4(0.30f, 0.30f, 0.32f, 1.00f);
    const ImVec4 textColor = ImVec4(0.95f, 0.95f, 0.96f, 1.00f);
    const ImVec4 checkMarkColor = ImVec4(0.70f, 0.70f, 0.72f, 1.00f);

    colors[ImGuiCol_WindowBg] = darkBg;
    colors[ImGuiCol_ChildBg] = mediumBg;
    colors[ImGuiCol_PopupBg] = mediumBg;

    colors[ImGuiCol_FrameBg] = lightBg;
    colors[ImGuiCol_FrameBgHovered] = accentColor;
    colors[ImGuiCol_FrameBgActive] = accentActive;

    colors[ImGuiCol_Text] = textColor;
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = accentColor;

    colors[ImGuiCol_TitleBg] = darkBg;
    colors[ImGuiCol_TitleBgActive] = darkBg;
    colors[ImGuiCol_TitleBgCollapsed] = darkBg;

    colors[ImGuiCol_Button] = accentColor;
    colors[ImGuiCol_ButtonHovered] = accentHover;
    colors[ImGuiCol_ButtonActive] = accentActive;

    colors[ImGuiCol_Tab] = mediumBg;
    colors[ImGuiCol_TabHovered] = accentHover;
    colors[ImGuiCol_TabActive] = accentColor;
    colors[ImGuiCol_TabUnfocused] = mediumBg;
    colors[ImGuiCol_TabUnfocusedActive] = accentColor;

    colors[ImGuiCol_SliderGrab] = accentHover;
    colors[ImGuiCol_SliderGrabActive] = accentActive;

    colors[ImGuiCol_CheckMark] = checkMarkColor;
    colors[ImGuiCol_FrameBg] = lightBg;

    colors[ImGuiCol_Header] = accentColor;
    colors[ImGuiCol_HeaderHovered] = accentHover;
    colors[ImGuiCol_HeaderActive] = accentActive;

    colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = accentHover;
    colors[ImGuiCol_SeparatorActive] = accentActive;

    colors[ImGuiCol_Border] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    colors[ImGuiCol_ScrollbarBg] = mediumBg;
    colors[ImGuiCol_ScrollbarGrab] = accentColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = accentHover;
    colors[ImGuiCol_ScrollbarGrabActive] = accentActive;

    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.35f);
    colors[ImGuiCol_NavHighlight] = accentHover;
    colors[ImGuiCol_DragDropTarget] = accentColor;

    style.WindowRounding = 8.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;

    style.ItemSpacing = ImVec2(10, 8);
    style.ItemInnerSpacing = ImVec2(6, 6);
    style.FramePadding = ImVec2(8, 6);
    style.WindowPadding = ImVec2(12, 12);

    style.GrabMinSize = 14.0f;
    style.ScrollbarSize = 12.0f;
}

void Menu::init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);

    ApplyImGuiTheme();

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 18.0f);
    smallFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 12.0f);

    std::cout << "initialized menu\n";
}

void Menu::render() {
    keybind.pollInputs();
    if (!showMenu) return;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);

    ImGui::Begin("Mozority | Beta Test", nullptr, window_flags);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
    ImGui::BeginChild("Sidebar", ImVec2(48, 0), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("MZ").x) * 0.5f);
    ImGui::Text("MZ");
    ImGui::PushFont(smallFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Beta").x) * 0.5f);
    ImGui::SameLine();
    ImGui::TextDisabled("Beta");
    ImGui::Separator();
    ImGui::PopFont();

    const char* tabIcons[] = { "A", "V", "M", "C" };
    const char* tabNames[] = { "Aim", "Visuals", "Misc", "Config" };

    for (int i = 0; i < 4; i++) {
        bool isSelected = (activeTab == i);
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
        }
        if (ImGui::Button(tabIcons[i], ImVec2(40, 40))) {
            activeTab = i;
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip(tabNames[i]);
        }
        if (isSelected) {
            ImGui::PopStyleColor();
        }
    }

    ImGui::PushFont(smallFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("BETA").x) * 0.5f);
    ImGui::TextDisabled("BETA");
    ImGui::PopFont();

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetTextLineHeight() - ImGui::GetStyle().WindowPadding.y);
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::SameLine();

    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.18f, 0.18f, 0.20f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.35f, 0.35f, 0.38f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.30f, 0.30f, 0.32f, 1.00f));

    switch (activeTab) {
    case 0: // Aim
    {
        const char* subTabs[] = { "Aimbot", "TriggerBot" };
        for (int i = 0; i < 2; i++) {
            bool isSelected = (aimSubTab == i);
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
            }
            if (ImGui::Button(subTabs[i], ImVec2(80, 30))) {
                aimSubTab = i;
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }
            if (i < 1) ImGui::SameLine(0, 5);
        }
        ImGui::NewLine();
        ImGui::Separator();

        if (aimSubTab == 0) { // Aimbot
            ImGui::Text("Aimbot Settings");
            ImGui::Separator();

            // Left Section
            ImGui::BeginChild("LeftSection", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 10, 0), true);
            ImGui::BeginGroup();
            ImGui::Checkbox("Enable##AimBot", &Config::aimbot);
            ImGui::SameLine();
            ImGui::Text("Key:");
            ImGui::SameLine();
            keybind.menuButton(Config::aimbot, false); // Render only key name and "Change" button
            // Explicit toggle/hold combobox below the button
            Keybind* aimbotKeybind = keybind.getKeybind(Config::aimbot);
            if (aimbotKeybind) {
                ImGui::PushItemWidth(80); // Match "Change" button size
                const char* keybindModes[] = { "Toggle", "Hold" };
                int modeIndex = static_cast<int>(aimbotKeybind->mode);
                if (ImGui::Combo("##AimbotMode", &modeIndex, keybindModes, IM_ARRAYSIZE(keybindModes))) {
                    aimbotKeybind->mode = static_cast<KeybindMode>(modeIndex);
                }
                ImGui::PopItemWidth();
            }
            ImGui::PushItemWidth(180);
            ImGui::SliderFloat("FOV", &Config::aimbot_fov, 0.f, 60.f, "%.1f");
            ImGui::Checkbox("Recoil Control", &Config::rcs);
            ImGui::SliderFloat("Smooth", &Config::aimbot_smoothness, 2.0f, 30.0f, "%.1f");
            ImGui::PopItemWidth();
            ImGui::EndGroup();
            ImGui::EndChild();

            ImGui::SameLine();

            // Right Section
            ImGui::BeginChild("RightSection", ImVec2(0, 0), true);
            ImGui::BeginGroup();
            ImGui::Checkbox("Wall Check", &Config::aimbot_wall_check);
            ImGui::SameLine();
            ImGui::TextDisabled("(!)");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted("May not work properly");
                ImGui::EndTooltip();
            }
            ImGui::Checkbox("Draw FOV Circle", &Config::fov_circle);
            if (Config::fov_circle) {
                ImGui::SameLine(0, 10);
                CustomColorPicker("Circle Color##FovColor", (float*)&Config::fovCircleColor);
            }
            ImGui::Checkbox("Multipoints", &Config::aimbot_multipoint);
            ImGui::PushItemWidth(180); // Match size of FOV and Smooth sliders
            ImGui::SliderInt("Multipoint Count", &Config::aimbot_multipoint_count, 1, 3, "%d");
            ImGui::SliderFloat("Max Aim Dist.", &Config::aimbot_max_distance, 250.0f, 10000.0f, "%.0f units");
            ImGui::PopItemWidth();
            ImGui::SameLine();
            ImGui::TextDisabled("(!)");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted("May not work properly");
                ImGui::EndTooltip();
            }
            ImGui::EndGroup();
            ImGui::EndChild();
        }
        else if (aimSubTab == 1) { // TriggerBot
            ImGui::Text("No additional settings");
        }
        break;
    }
    case 1: // Visuals
    {
        const char* subTabs[] = { "ESP", "World", "Chams", "Local", "Removals" };
        for (int i = 0; i < 5; i++) {
            bool isSelected = (visualsSubTab == i);
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
            }
            if (ImGui::Button(subTabs[i], ImVec2(80, 30))) {
                visualsSubTab = i;
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }
            if (i < 4) ImGui::SameLine(0, 5);
        }
        ImGui::NewLine();
        ImGui::Separator();

        if (visualsSubTab == 0) { // Player ESP
            ImGui::BeginGroup();
            ImGui::Text("Box Settings");
            ImGui::Separator();
            ImGui::Checkbox("Box", &Config::esp);
            if (Config::esp) {
                CustomColorPicker("ESP Color##BoxColor", (float*)&Config::espColor);
                ImGui::PushItemWidth(180);
                ImGui::SliderFloat("Thickness", &Config::espThickness, 1.0f, 5.0f);
                ImGui::PopItemWidth();
                ImGui::Checkbox("Box Fill", &Config::espFill);
                if (Config::espFill) {
                    ImGui::PushItemWidth(180);
                    ImGui::SliderFloat("Fill Opacity", &Config::espFillOpacity, 0.0f, 1.0f);
                    ImGui::PopItemWidth();
                }
            }
            ImGui::EndGroup();
            ImGui::Spacing();

            ImGui::BeginGroup();
            ImGui::Text("Other Settings");
            ImGui::Separator();
            ImGui::Checkbox("Team Check", &Config::teamCheck);
            ImGui::Checkbox("Health Bar", &Config::showHealth);
            ImGui::Checkbox("Name Tags", &Config::showNameTags);
            ImGui::EndGroup();
            ImGui::Spacing();
        }
        else if (visualsSubTab == 1) { // World
            ImGui::BeginGroup();
            ImGui::Text("World Settings");
            ImGui::Separator();
            ImGui::Checkbox("Night Mode", &Config::Night);
            if (Config::Night) {
                CustomColorPicker("Night Color", (float*)&Config::NightColor);
            }
            ImGui::Checkbox("Custom FOV", &Config::fovEnabled);
            if (Config::fovEnabled) {
                ImGui::PushItemWidth(180);
                ImGui::SliderFloat("FOV Value##FovSlider", &Config::fov, 20.0f, 160.0f, "%1.0f");
                ImGui::PopItemWidth();
            }
            ImGui::EndGroup();
            ImGui::Spacing();
        }
        else if (visualsSubTab == 2) { // Chams
            ImGui::BeginGroup();
            ImGui::Text("Chams Settings");
            ImGui::Separator();
            ImGui::Checkbox("Chams##ChamsCheckbox", &Config::enemyChams);
            if (Config::enemyChams) {
                ImGui::PushItemWidth(150);
                const char* chamsMaterials[] = { "Flat", "Illuminate", "Glow" };
                ImGui::Combo("Material", &Config::chamsMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
                ImGui::PopItemWidth();
                CustomColorPicker("Chams Color##ChamsColor", (float*)&Config::colVisualChams);
            }
            ImGui::Checkbox("Chams-XQZ", &Config::enemyChamsInvisible);
            if (Config::enemyChamsInvisible) {
                CustomColorPicker("XQZ Color##ChamsXQZColor", (float*)&Config::colVisualChamsIgnoreZ);
            }
            ImGui::EndGroup();
            ImGui::Spacing();
        }
        else if (visualsSubTab == 3) { // Hand Chams
            ImGui::BeginGroup();
            ImGui::Text("Hand Chams Settings");
            ImGui::Separator();
            ImGui::Checkbox("Hand Chams##HandChamsCheckbox", &Config::armChams);
            if (Config::armChams) {
                CustomColorPicker("Hand Color##HandChamsColor", (float*)&Config::colArmChams);
            }
            ImGui::Checkbox("Viewmodel Chams", &Config::viewmodelChams);
            if (Config::viewmodelChams) {
                CustomColorPicker("Viewmodel Color##ViewModelChamsColor", (float*)&Config::colViewmodelChams);
            }
            ImGui::EndGroup();
            ImGui::Spacing();
        }
        else if (visualsSubTab == 4) { // Removals
            ImGui::BeginGroup();
            ImGui::Text("Removals Settings");
            ImGui::Separator();
            ImGui::Checkbox("Anti Flash", &Config::antiflash);
            ImGui::EndGroup();
            ImGui::Spacing();
        }
        break;
    }
    case 2: // Misc
    {
        const char* subTabs[] = { "Movement", "Other" };
        for (int i = 0; i < 2; i++) {
            bool isSelected = (miscSubTab == i);
            if (isSelected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
            }
            if (ImGui::Button(subTabs[i], ImVec2(80, 30))) {
                miscSubTab = i;
            }
            if (isSelected) {
                ImGui::PopStyleColor();
            }
            if (i < 1) ImGui::SameLine(0, 5);
        }
        ImGui::NewLine();
        ImGui::Separator();

        if (miscSubTab == 0) { // Movement
            ImGui::Text("No additional settings");
        }
        else if (miscSubTab == 1) { // Other
            ImGui::BeginGroup();
            ImGui::Text("Other Settings");
            ImGui::Separator();
            ImGui::Checkbox("Show Keybinds", &Config::show_keybinds);
            ImGui::Checkbox("Radar Hack", &Config::radar);
            ImGui::SameLine();
            ImGui::TextDisabled("(?)");
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::TextUnformatted("Not implemented yet");
                ImGui::EndTooltip();
            }
            ImGui::EndGroup();
            ImGui::Spacing();
        }
        break;
    }
    case 3: // Config
    {
        static char configName[128] = "";
        static std::vector<std::string> configList = internal_config::ConfigManager::ListConfigs();
        static int selectedConfigIndex = -1;

        ImGui::InputText("Config Name", configName, IM_ARRAYSIZE(configName));

        if (ImGui::Button("Refresh")) {
            configList = internal_config::ConfigManager::ListConfigs();
        }
        ImGui::SameLine();
        if (ImGui::Button("Load")) {
            internal_config::ConfigManager::Load(configName);
        }
        ImGui::SameLine();
        if (ImGui::Button("Save")) {
            internal_config::ConfigManager::Save(configName);
            configList = internal_config::ConfigManager::ListConfigs();
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            internal_config::ConfigManager::Remove(configName);
            configList = internal_config::ConfigManager::ListConfigs();
        }

        ImGui::Separator();
        ImGui::Text("Saved Configs");
        ImGui::BeginChild("ConfigList", ImVec2(0, 200), true);
        for (int i = 0; i < static_cast<int>(configList.size()); i++) {
            if (ImGui::Selectable(configList[i].c_str(), selectedConfigIndex == i)) {
                selectedConfigIndex = i;
                strncpy_s(configName, sizeof(configName), configList[i].c_str(), _TRUNCATE);
            }
        }
        ImGui::EndChild();
        break;
    }
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(3);
    ImGui::EndChild();
    ImGui::End();
}

void Menu::toggleMenu() {
    showMenu = !showMenu;
}