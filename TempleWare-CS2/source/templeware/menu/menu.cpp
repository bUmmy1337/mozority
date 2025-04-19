#include "menu.h"
#include "../config/config.h"

#include <iostream>
#include <vector>
#include "../config/configmanager.h"

#include "../keybinds/keybinds.h"

#include "../utils/logging/log.h"

void ApplyImGuiTheme() {
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Основные цвета
    ImVec4 darkColor = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
    ImVec4 darkerColor = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
    ImVec4 darkPanelColor = ImVec4(0.12f, 0.12f, 0.12f, 1.0f); // Новый цвет для панелей
    ImVec4 redAccent = ImVec4(0.75f, 0.15f, 0.15f, 1.0f);
    ImVec4 redAccentLight = ImVec4(0.85f, 0.25f, 0.25f, 1.0f);
    ImVec4 redAccentHover = ImVec4(0.85f, 0.25f, 0.25f, 1.0f);
    ImVec4 redAccentActive = ImVec4(0.65f, 0.05f, 0.05f, 1.0f);
    ImVec4 inputFieldColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f); // Цвет полей ввода

    // Фон и основные элементы
    colors[ImGuiCol_WindowBg] = darkColor;
    colors[ImGuiCol_ChildBg] = darkPanelColor; // Более светлый фон для дочерних элементов
    colors[ImGuiCol_PopupBg] = darkColor;

    // Фреймы и элементы управления
    colors[ImGuiCol_FrameBg] = inputFieldColor; // Более контрастный фон для полей ввода/слайдеров
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);

    // Текст
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Заголовки
    colors[ImGuiCol_TitleBg] = darkerColor;
    colors[ImGuiCol_TitleBgActive] = darkerColor;
    colors[ImGuiCol_TitleBgCollapsed] = darkerColor;

    // Кнопки
    colors[ImGuiCol_Button] = redAccent;
    colors[ImGuiCol_ButtonHovered] = redAccentHover;
    colors[ImGuiCol_ButtonActive] = redAccentActive;

    // Вкладки
    colors[ImGuiCol_Tab] = darkerColor;
    colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = darkerColor;
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);

    // Ползунки
    colors[ImGuiCol_SliderGrab] = redAccent;
    colors[ImGuiCol_SliderGrabActive] = redAccentActive;

    // Чекбоксы
    colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f); // Белая галочка
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Более светлый фон для чекбоксов
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

    // Поля ввода текста
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.75f, 0.15f, 0.15f, 0.50f);
    colors[ImGuiCol_FrameBg] = inputFieldColor; // Яркий фон для полей ввода

    // Заголовки секций
    colors[ImGuiCol_Header] = redAccent;
    colors[ImGuiCol_HeaderHovered] = redAccentHover;
    colors[ImGuiCol_HeaderActive] = redAccentActive;

    // Разделители
    colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
    colors[ImGuiCol_SeparatorHovered] = redAccent;
    colors[ImGuiCol_SeparatorActive] = redAccentActive;

    // Границы
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // Скроллбар
    colors[ImGuiCol_ScrollbarBg] = darkerColor;
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = redAccentLight;
    colors[ImGuiCol_ScrollbarGrabActive] = redAccent;

    // Стиль элементов
    style.WindowRounding = 3.0f;
    style.ChildRounding = 3.0f;
    style.FrameRounding = 2.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 3.0f;

    // Границы
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f; // Добавляем границу к фреймам
    style.TabBorderSize = 0.0f;

    // Отступы
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.FramePadding = ImVec2(6, 4);
    style.WindowPadding = ImVec2(8, 8);

    // Размеры элементов
    style.GrabMinSize = 12.0f;
    style.ScrollbarSize = 14.0f;
    style.FrameBorderSize = 1.0f; // Граница вокруг элементов управления
}

Menu::Menu() {
    activeTab = 0;
    showMenu = true;
}

void Menu::init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);

    ApplyImGuiTheme();

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);

    std::cout << "initialized menu\n";
}

void Menu::render() {
    keybind.pollInputs();
    if (showMenu) {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;

        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);

        ImGui::Begin("Mozority | Beta Test", nullptr, window_flags);

        {
            float windowWidth = ImGui::GetWindowWidth();
            float rightTextWidth = ImGui::CalcTextSize("mozority.pro").x;

            ImGui::Text("Mozority - Beta Test");

            ImGui::SameLine(windowWidth - rightTextWidth - 10);
            ImGui::Text("mozority.pro");
        }

        ImGui::Separator();

        const char* tabNames[] = { "Aim", "Visuals", "Misc", "Config" };

        if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_NoTooltip)) {
            for (int i = 0; i < 4; i++) {
                if (ImGui::BeginTabItem(tabNames[i])) {
                    activeTab = i;
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        ImGui::BeginChild("ContentRegion", ImVec2(0, 0), false);

        switch (activeTab) {
        case 0:
        {
            ImGui::BeginChild("AimLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("General");
            ImGui::Separator();

            ImGui::Checkbox("Enable##AimBot", &Config::aimbot);
            ImGui::SameLine();
            ImGui::Text("Key:");
            ImGui::SameLine();
            keybind.menuButton(Config::aimbot);

            ImGui::SliderFloat("FOV", &Config::aimbot_fov, 0.f, 60.f);
            ImGui::Checkbox("Draw FOV Circle", &Config::fov_circle);
            if (Config::fov_circle) {
                ImGui::ColorEdit4("Circle Color##FovColor", (float*)&Config::fovCircleColor);
            }
            ImGui::Checkbox("Recoil Control", &Config::rcs);

            // Smoothing option
            ImGui::SliderFloat("Smooth", &Config::aimbot_smoothness, 0.0f, 30.0f, "%.1f");

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("AimRight", ImVec2(0, 0), true);
            ImGui::Text("TriggerBot");
            ImGui::Separator();
            ImGui::Text("No additional settings");

            ImGui::EndChild();
        }
        break;

        case 1:
        {
            ImGui::BeginChild("VisualsLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("Player ESP");
            ImGui::Separator();

            ImGui::Checkbox("Box", &Config::esp);
            ImGui::SliderFloat("Thickness", &Config::espThickness, 1.0f, 5.0f);
            ImGui::Checkbox("Box Fill", &Config::espFill);
            if (Config::espFill) {
                ImGui::SliderFloat("Fill Opacity", &Config::espFillOpacity, 0.0f, 1.0f);
            }
            ImGui::ColorEdit4("ESP Color##BoxColor", (float*)&Config::espColor);
            ImGui::Checkbox("Team Check", &Config::teamCheck);
            ImGui::Checkbox("Health Bar", &Config::showHealth);
            ImGui::Checkbox("Name Tags", &Config::showNameTags);

            ImGui::Spacing();
            ImGui::Text("World");
            ImGui::Separator();

            ImGui::Checkbox("Night Mode", &Config::Night);
            if (Config::Night) {
                ImGui::ColorEdit4("Night Color", (float*)&Config::NightColor);
            }

            ImGui::Checkbox("Custom FOV", &Config::fovEnabled);
            if (Config::fovEnabled) {
                ImGui::SliderFloat("FOV Value##FovSlider", &Config::fov, 20.0f, 160.0f, "%1.0f");
            }

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("VisualsRight", ImVec2(0, 0), true);
            ImGui::Text("Chams");
            ImGui::Separator();

            ImGui::Checkbox("Chams##ChamsCheckbox", &Config::enemyChams);
            const char* chamsMaterials[] = { "Flat", "Illuminate", "Glow" };
            ImGui::Combo("Material", &Config::chamsMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
            if (Config::enemyChams) {
                ImGui::ColorEdit4("Chams Color##ChamsColor", (float*)&Config::colVisualChams);
            }
            ImGui::Checkbox("Chams-XQZ", &Config::enemyChamsInvisible);
            if (Config::enemyChamsInvisible) {
                ImGui::ColorEdit4("XQZ Color##ChamsXQZColor", (float*)&Config::colVisualChamsIgnoreZ);
            }

            ImGui::Spacing();
            ImGui::Text("Hand Chams");
            ImGui::Separator();

            ImGui::Checkbox("Hand Chams", &Config::armChams);
            if (Config::armChams) {
                ImGui::ColorEdit4("Hand Color##HandChamsColor", (float*)&Config::colArmChams);
            }
            ImGui::Checkbox("Viewmodel Chams", &Config::viewmodelChams);
            if (Config::viewmodelChams) {
                ImGui::ColorEdit4("Viewmodel Color##ViewModelChamsColor", (float*)&Config::colViewmodelChams);
            }

            ImGui::Spacing();
            ImGui::Text("Removals");
            ImGui::Separator();

            ImGui::Checkbox("Anti Flash", &Config::antiflash);

            ImGui::EndChild();
        }
        break;

        case 2:
        {
            ImGui::BeginChild("MiscLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("Movement");
            ImGui::Separator();

            ImGui::Text("No additional settings");

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("MiscRight", ImVec2(0, 0), true);
            ImGui::Text("Other");
            ImGui::Separator();

            ImGui::Text("No additional settings");

            ImGui::EndChild();
        }
        break;

        case 3:
        {
            ImGui::BeginChild("ConfigLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 5, 0), true);
            ImGui::Text("General");
            ImGui::Separator();

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

            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("ConfigRight", ImVec2(0, 0), true);
            ImGui::Text("Saved Configs");
            ImGui::Separator();

            for (int i = 0; i < static_cast<int>(configList.size()); i++) {
                if (ImGui::Selectable(configList[i].c_str(), selectedConfigIndex == i)) {
                    selectedConfigIndex = i;
                    strncpy_s(configName, sizeof(configName), configList[i].c_str(), _TRUNCATE);
                }
            }

            ImGui::EndChild();
        }
        break;
        }

        ImGui::EndChild();
        ImGui::End();
    }
}

void Menu::toggleMenu() {
    showMenu = !showMenu;
}