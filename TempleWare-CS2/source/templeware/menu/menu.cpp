#define STB_IMAGE_IMPLEMENTATION
#include "menu_utils/stb_image.h"
#include "menu.h"
#include "../config/config.h"

#include <iostream>
#include <vector>
#include "../config/configmanager.h"

#include "../keybinds/keybinds.h"

#include "../utils/logging/log.h"
#include <d3d11.h>
#include <string>
#include <cmath>
#include "menu_utils/aimsettings.h"
#include "menu_utils/world.h"       // Visuals icon (world[])
#include "menu_utils/mmain.h"       // Misc icon (mmain[])
#include "menu_utils/configs.h"




// Helper function for animations
static float Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Custom button with image support
static bool CustomImageButton(ID3D11ShaderResourceView* texture, const char* label, const ImVec2& size, const ImVec2& iconSize, bool selected = false) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
    ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.20f, 0.55f, 0.85f, 1.0f) : ImVec4(0.15f, 0.15f, 0.18f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.65f, 0.95f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.45f, 0.75f, 1.0f));

    bool clicked = ImGui::Button(label, size);
    if (texture) {
        ImVec2 pos = ImGui::GetItemRectMin();
        ImVec2 center = ImVec2(pos.x + size.x * 0.5f - iconSize.x * 0.5f, pos.y + size.y * 0.5f - iconSize.y * 0.5f);
        ImGui::GetWindowDrawList()->AddImage(texture, center, ImVec2(center.x + iconSize.x, center.y + iconSize.y));
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
    return clicked;
}

// Custom color picker with glassmorphism
static void CustomColorPicker(const char* label, float col[4]) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));

    if (ImGui::ColorButton(label, ImVec4(col[0], col[1], col[2], col[3]), ImGuiColorEditFlags_AlphaPreview)) {
        ImGui::OpenPopup(label);
    }

    if (ImGui::BeginPopup(label)) {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.12f, 0.95f));
        ImGui::PushItemWidth(220);
        ImGui::ColorPicker4("##picker", col, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::PopItemWidth();
        ImGui::PopStyleColor();
        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(3);
}

// Custom slider with gradient track
static bool CustomSliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.1f") {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.12f, 0.14f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.20f, 0.55f, 0.85f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.25f, 0.65f, 0.95f, 1.0f));

    bool changed = ImGui::SliderFloat(label, v, v_min, v_max, format);

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
    return changed;
}

// Apply ImGui theme
void Menu::ApplyImGuiTheme() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    const ImVec4 bgDark = ImVec4(0.08f, 0.08f, 0.10f, 0.98f);
    const ImVec4 bgMedium = ImVec4(0.12f, 0.12f, 0.14f, 0.95f);
    const ImVec4 bgLight = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    const ImVec4 accent = ImVec4(0.20f, 0.55f, 0.85f, 1.0f);
    const ImVec4 accentHover = ImVec4(0.25f, 0.65f, 0.95f, 1.0f);
    const ImVec4 accentActive = ImVec4(0.15f, 0.45f, 0.75f, 1.0f);
    const ImVec4 text = ImVec4(0.95f, 0.95f, 0.98f, 1.0f);
    const ImVec4 secondaryAccent = ImVec4(0.60f, 0.20f, 0.80f, 1.0f);

    colors[ImGuiCol_WindowBg] = bgDark;
    colors[ImGuiCol_ChildBg] = bgMedium;
    colors[ImGuiCol_PopupBg] = bgMedium;
    colors[ImGuiCol_FrameBg] = bgLight;
    colors[ImGuiCol_FrameBgHovered] = accent;
    colors[ImGuiCol_FrameBgActive] = accentActive;
    colors[ImGuiCol_Text] = text;
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.62f, 1.0f);
    colors[ImGuiCol_TextSelectedBg] = accent;
    colors[ImGuiCol_TitleBg] = bgDark;
    colors[ImGuiCol_TitleBgActive] = bgDark;
    colors[ImGuiCol_TitleBgCollapsed] = bgDark;
    colors[ImGuiCol_Button] = accent;
    colors[ImGuiCol_ButtonHovered] = accentHover;
    colors[ImGuiCol_ButtonActive] = accentActive;
    colors[ImGuiCol_Tab] = bgMedium;
    colors[ImGuiCol_TabHovered] = accentHover;
    colors[ImGuiCol_TabActive] = accent;
    colors[ImGuiCol_TabUnfocused] = bgMedium;
    colors[ImGuiCol_TabUnfocusedActive] = accent;
    colors[ImGuiCol_SliderGrab] = accentHover;
    colors[ImGuiCol_SliderGrabActive] = accentActive;
    colors[ImGuiCol_CheckMark] = secondaryAccent;
    colors[ImGuiCol_Header] = accent;
    colors[ImGuiCol_HeaderHovered] = accentHover;
    colors[ImGuiCol_HeaderActive] = accentActive;
    colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    colors[ImGuiCol_SeparatorHovered] = accentHover;
    colors[ImGuiCol_SeparatorActive] = accentActive;
    colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.12f, 0.5f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_ScrollbarBg] = bgMedium;
    colors[ImGuiCol_ScrollbarGrab] = accent;
    colors[ImGuiCol_ScrollbarGrabHovered] = accentHover;
    colors[ImGuiCol_ScrollbarGrabActive] = accentActive;
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.4f);
    colors[ImGuiCol_NavHighlight] = accentHover;
    colors[ImGuiCol_DragDropTarget] = accent;

    style.WindowRounding = 12.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(12, 10);
    style.ItemInnerSpacing = ImVec2(8, 8);
    style.FramePadding = ImVec2(10, 8);
    style.WindowPadding = ImVec2(16, 16);
    style.GrabMinSize = 16.0f;
    style.ScrollbarSize = 14.0f;
    style.Alpha = 1.0f;
}

Menu::Menu() : keybind(::keybind) {
    showMenu = true;
    activeTab = 0;
    aimSubTab = 0;
    visualsSubTab = 0;
    miscSubTab = 0;
    smallFont = nullptr;
    for (int i = 0; i < 4; i++) {
        tabIconsTextures[i] = nullptr;
    }
    logoTexture = nullptr;
    iconSize = ImVec2(15, 15);
    logoSize = ImVec2(15, 15);
}

Menu::~Menu() {
    for (int i = 0; i < 4; i++) {
        if (tabIconsTextures[i]) {
            tabIconsTextures[i]->Release();
        }
    }
    if (logoTexture) {
        logoTexture->Release();
    }
}

bool Menu::LoadTextureFromMemory(unsigned char* data, size_t dataSize, ID3D11Device* device, ID3D11ShaderResourceView** outSrv, int* outWidth, int* outHeight) {
    int width, height, channels;
    unsigned char* imageData = stbi_load_from_memory(data, dataSize, &width, &height, &channels, 4);
    if (!imageData) {
        std::cerr << "Failed to load image from memory: " << stbi_failure_reason() << std::endl;
        return false;
    }

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA subResource = {};
    subResource.pSysMem = imageData;
    subResource.SysMemPitch = width * 4;

    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = device->CreateTexture2D(&desc, &subResource, &texture);
    stbi_image_free(imageData);
    if (FAILED(hr)) {
        std::cerr << "Failed to create texture: " << std::hex << hr << std::endl;
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(texture, &srvDesc, outSrv);
    texture->Release();
    if (FAILED(hr)) {
        std::cerr << "Failed to create shader resource view: " << std::hex << hr << std::endl;
        return false;
    }

    *outWidth = width;
    *outHeight = height;
    return true;
}

void Menu::init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);

    ApplyImGuiTheme();

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Montserrat-Regular.ttf", 18.0f);
    smallFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Montserrat-Light.ttf", 14.0f);

    // Load tab icons
    struct IconData {
        unsigned char* data;
        size_t size;
        const char* name;
    };
    IconData icons[] = {
        { legit, sizeof(legit), "Aimbot" },
        { world, sizeof(world), "Visuals" },
        { mmain, sizeof(mmain), "Misc" },
        { configs, sizeof(configs), "Config" }
    };

    int maxWidth = 15, maxHeight = 15;
    for (int i = 0; i < 4; i++) {
        int width, height;
        if (LoadTextureFromMemory(icons[i].data, icons[i].size, pDevice, &tabIconsTextures[i], &width, &height)) {
            std::cout << "Loaded " << icons[i].name << " icon (" << width << "x" << height << ")\n";
            maxWidth = std::max(maxWidth, width);
            maxHeight = std::max(maxHeight, height);
        }
        else {
            std::cerr << "Failed to load " << icons[i].name << " icon\n";
        }
    }
    iconSize = ImVec2(static_cast<float>(maxWidth), static_cast<float>(maxHeight));

    // Load logo texture (using legit[] as placeholder)
    int logoWidth, logoHeight;
    if (LoadTextureFromMemory(legit, sizeof(legit), pDevice, &logoTexture, &logoWidth, &logoHeight)) {
        logoSize = ImVec2(static_cast<float>(logoWidth), static_cast<float>(logoHeight));
        std::cout << "Loaded logo icon (" << logoWidth << "x" << logoHeight << ")\n";
    }
    else {
        std::cerr << "Failed to load logo icon\n";
    }

    std::cout << "Initialized menu\n";
}

void Menu::render() {
    keybind.pollInputs();
    if (!showMenu) return;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);

    ImGui::Begin("Mozority | Premium", nullptr, window_flags);

    // Sidebar
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    ImGui::BeginChild("Sidebar", ImVec2(60, 0), true, ImGuiWindowFlags_NoScrollbar);
    if (logoTexture) {
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - logoSize.x) * 0.5f);
        ImGui::Image(logoTexture, logoSize);
    }
    ImGui::PushFont(smallFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("PREMIUM").x) * 0.5f);
    ImGui::TextDisabled("PREMIUM");
    ImGui::Separator();
    ImGui::PopFont();

    const char* tabNames[] = { "Aimbot", "Visuals", "Misc", "Config" };

    for (int i = 0; i < 4; i++) {
        bool isSelected = (activeTab == i);
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));
        }
        char buttonId[32];
        snprintf(buttonId, sizeof(buttonId), "##Tab%d", i);
        if (CustomImageButton(tabIconsTextures[i], buttonId, ImVec2(44, 44), iconSize, isSelected)) {
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
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetTextLineHeight() - ImGui::GetStyle().WindowPadding.y);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("v1.0").x) * 0.5f);
    ImGui::TextDisabled("v1.0");
    ImGui::PopFont();

    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::SameLine();

    // Main Content
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.12f, 0.12f, 0.14f, 0.95f));

    switch (activeTab) {
    case 0: // Aim
    {
        const char* subTabs[] = { "Aimbot", "TriggerBot" };
        for (int i = 0; i < 2; i++) {
            bool isSelected = (aimSubTab == i);
            if (CustomImageButton(nullptr, subTabs[i], ImVec2(100, 32), ImVec2(0, 0), isSelected)) {
                aimSubTab = i;
            }
            if (i < 1) ImGui::SameLine(0, 8);
        }
        ImGui::Separator();

        if (aimSubTab == 0) { // Aimbot
            ImGui::Text("Aimbot Configuration");
            ImGui::Separator();

            ImGui::BeginChild("LeftSection", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 12, 0), true);
            ImGui::Checkbox("Enable Aimbot", &Config::aimbot);
            ImGui::SameLine();
            ImGui::Text("Key:");
            ImGui::SameLine();
            keybind.menuButton(Config::aimbot, false);
            Keybind* aimbotKeybind = keybind.getKeybind(Config::aimbot);
            if (aimbotKeybind) {
                ImGui::PushItemWidth(100);
                const char* keybindModes[] = { "Toggle", "Hold" };
                int modeIndex = static_cast<int>(aimbotKeybind->mode);
                if (ImGui::Combo("Mode##AimbotMode", &modeIndex, keybindModes, IM_ARRAYSIZE(keybindModes))) {
                    aimbotKeybind->mode = static_cast<KeybindMode>(modeIndex);
                }
                ImGui::PopItemWidth();
            }
            CustomSliderFloat("FOV", &Config::aimbot_fov, 0.f, 60.f);
            ImGui::Checkbox("Recoil Compensation", &Config::rcs);
            CustomSliderFloat("Smoothness", &Config::aimbot_smoothness, 0.0f, 30.0f);
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("RightSection", ImVec2(0, 0), true);
            ImGui::Checkbox("Wall Check", &Config::aimbot_wall_check);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("May have inconsistent behavior");
            }
            ImGui::Checkbox("Draw FOV Circle", &Config::fov_circle);
            if (Config::fov_circle) {
                ImGui::SameLine();
                CustomColorPicker("FOV Circle Color", (float*)&Config::fovCircleColor);
            }
            ImGui::Checkbox("Multipoint Targeting", &Config::aimbot_multipoint);
            ImGui::PushItemWidth(200);
            ImGui::SliderInt("Multipoint Count", &Config::aimbot_multipoint_count, 1, 3, "%d");
            CustomSliderFloat("Max Distance", &Config::aimbot_max_distance, 250.0f, 10000.0f, "%.0f units");
            ImGui::PopItemWidth();
            ImGui::EndChild();
        }
        else if (aimSubTab == 1) { // TriggerBot
            ImGui::Text("TriggerBot Settings");
            ImGui::Separator();
            ImGui::Text("No additional settings available.");
        }
        break;
    }
    case 1: // Visuals
    {
        const char* subTabs[] = { "ESP", "World", "Chams", "Local", "Removals" };
        for (int i = 0; i < 5; i++) {
            bool isSelected = (visualsSubTab == i);
            if (CustomImageButton(nullptr, subTabs[i], ImVec2(100, 32), ImVec2(0, 0), isSelected)) {
                visualsSubTab = i;
            }
            if (i < 4) ImGui::SameLine(0, 8);
        }
        ImGui::Separator();

        if (visualsSubTab == 0) { // Player ESP
            ImGui::Text("ESP Settings");
            ImGui::Separator();
            ImGui::Checkbox("Enable Box ESP", &Config::esp);
            if (Config::esp) {
                CustomColorPicker("Box Color", (float*)&Config::espColor);
                CustomSliderFloat("Box Thickness", &Config::espThickness, 1.0f, 5.0f);
                ImGui::Checkbox("Box Fill", &Config::espFill);
                if (Config::espFill) {
                    CustomSliderFloat("Fill Opacity", &Config::espFillOpacity, 0.0f, 1.0f);
                }
            }
            ImGui::Checkbox("Team Check", &Config::teamCheck);
            ImGui::Checkbox("Health Bar", &Config::showHealth);
            ImGui::Checkbox("Name Tags", &Config::showNameTags);
        }
        else if (visualsSubTab == 1) { // World
            ImGui::Text("World Settings");
            ImGui::Separator();
            ImGui::Checkbox("Night Mode", &Config::Night);
            if (Config::Night) {
                CustomColorPicker("Night Color", (float*)&Config::NightColor);
            }
            ImGui::Checkbox("Custom FOV", &Config::fovEnabled);
            if (Config::fovEnabled) {
                CustomSliderFloat("FOV Value", &Config::fov, 20.0f, 160.0f, "%1.0f");
            }
        }
        else if (visualsSubTab == 2) { // Chams
            ImGui::Text("Chams Settings | Disabled");
            ImGui::Separator();
            ImGui::Checkbox("Enemy Chams", &Config::enemyChams);
            if (Config::enemyChams) {
                ImGui::PushItemWidth(150);
                const char* chamsMaterials[] = { "Flat", "Illuminate", "Glow" };
                ImGui::Combo("Material", &Config::chamsMaterial, chamsMaterials, IM_ARRAYSIZE(chamsMaterials));
                ImGui::PopItemWidth();
                CustomColorPicker("Chams Color", (float*)&Config::colVisualChams);
            }
            ImGui::Checkbox("Chams-XQZ", &Config::enemyChamsInvisible);
            if (Config::enemyChamsInvisible) {
                CustomColorPicker("XQZ Color", (float*)&Config::colVisualChamsIgnoreZ);
            }
        }
        else if (visualsSubTab == 3) { // Hand Chams
            ImGui::Text("Hand Chams Settings");
            ImGui::Separator();
            ImGui::Checkbox("Hand Chams", &Config::armChams);
            if (Config::armChams) {
                CustomColorPicker("Hand Color", (float*)&Config::colArmChams);
            }
            ImGui::Checkbox("Viewmodel Chams", &Config::viewmodelChams);
            if (Config::viewmodelChams) {
                CustomColorPicker("Viewmodel Color", (float*)&Config::colViewmodelChams);
            }
        }
        else if (visualsSubTab == 4) { // Removals
            ImGui::Text("Removals Settings");
            ImGui::Separator();
            ImGui::Checkbox("Anti-Flash", &Config::antiflash);
        }
        break;
    }
    case 2: // Misc
    {
        const char* subTabs[] = { "Movement", "Other" };
        for (int i = 0; i < 2; i++) {
            bool isSelected = (miscSubTab == i);
            if (CustomImageButton(nullptr, subTabs[i], ImVec2(100, 32), ImVec2(0, 0), isSelected)) {
                miscSubTab = i;
            }
            if (i < 1) ImGui::SameLine(0, 8);
        }
        ImGui::Separator();

        if (miscSubTab == 0) { // Movement
            ImGui::Text("Movement Settings");
            ImGui::Separator();
            ImGui::Text("No additional settings available.");
        }
        else if (miscSubTab == 1) { // Other
            ImGui::Text("Other Settings");
            ImGui::Separator();
            ImGui::Checkbox("Show Keybinds", &Config::show_keybinds);
            ImGui::Checkbox("Radar Hack", &Config::radar);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Feature not yet implemented");
            }
        }
        break;
    }
    case 3: // Config
    {
        static char configName[128] = "";
        static std::vector<std::string> configList = internal_config::ConfigManager::ListConfigs();
        static int selectedConfigIndex = -1;

        ImGui::Text("Configuration Management");
        ImGui::Separator();
        ImGui::InputText("Config Name", configName, IM_ARRAYSIZE(configName));
        if (CustomImageButton(nullptr, "Refresh", ImVec2(100, 32), ImVec2(0, 0))) {
            configList = internal_config::ConfigManager::ListConfigs();
        }
        ImGui::SameLine();
        if (CustomImageButton(nullptr, "Load", ImVec2(100, 32), ImVec2(0, 0))) {
            internal_config::ConfigManager::Load(configName);
        }
        ImGui::SameLine();
        if (CustomImageButton(nullptr, "Save", ImVec2(100, 32), ImVec2(0, 0))) {
            internal_config::ConfigManager::Save(configName);
            configList = internal_config::ConfigManager::ListConfigs();
        }
        ImGui::SameLine();
        if (CustomImageButton(nullptr, "Delete", ImVec2(100, 32), ImVec2(0, 0))) {
            internal_config::ConfigManager::Remove(configName);
            configList = internal_config::ConfigManager::ListConfigs();
        }

        ImGui::Separator();
        ImGui::Text("Saved Configurations");
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

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
    ImGui::EndChild();
    ImGui::End();
}

void Menu::toggleMenu() {
    showMenu = !showMenu;
}