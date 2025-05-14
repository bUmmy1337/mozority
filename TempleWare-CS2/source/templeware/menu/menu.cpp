#include "menu.h"
#include "../config/config.h"

#include <iostream>
#include <vector>
#include "../config/configmanager.h"

#include "../keybinds/keybinds.h"

#include "../utils/logging/log.h"

// Fonts from onetap_menu.cpp
ImFont* segoe_ui_semibold = nullptr;
ImFont* segoe_ui_bold = nullptr;

// Key strings from onetap_menu.cpp
static const char* const key_str[] = {
    "none", "mouse 1", "mouse 2", "cancel", "mouse 3", "mouse 4", "mouse 5", "unknown",
    "backspace", "tab", "Unknown", "Unknown", "clear", "enter", "Unknown", "Unknown",
    "shift", "ctrl", "alt", "pause break", "caps lock", "kana", "unknown", "junja",
    "final", "kanji", "unknown", "escape", "con", "VK_NONCONVERT", "ACCEPT", "M CHANGE",
    "space", "PGUP", "PGDOWN", "end", "home", "left", "up", "right", "down", "select",
    "print", "execute", "snap", "insert", "delete", "help", "0", "1", "2", "3", "4",
    "5", "6", "7", "8", "9", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown",
    "Unknown", "Unknown", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l",
    "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "LWIN",
    "RWIN", "APPS", "Unknown", "SLEEP", "numpad 1", "numpad 2", "numpad 3", "numpad 4",
    "numpad 4", "numpad 5", "numpad 6", "numpad 7", "numpad 8", "numpad 9", "multiply",
    "add", "separator", "substract", "decimal", "divide", "f1", "f2", "f3", "f4", "f5",
    "f6", "f7", "f8", "f9", "f10", "f11", "f12", "F13", "F14", "F15", "F16", "F17",
    "F18", "F19", "F20", "F21", "F22", "F23", "F24", "Unknown", "Unknown", "Unknown",
    "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "numlock", "scroll lock",
    "OEM_NEC_EQUAL", "OEM_FJ_MASSHOU", "OEM_FJ_TOUROKU", "OEM_FJ_LOYA", "OEM_FJ_ROYA",
    "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown", "Unknown",
    "Unknown", "Unknown", "left shift", "right shift", "left ctrl", "right ctrl",
    "left alt", "right alt"
};

// Colors from onetap_menu.cpp
ImColor menu_background_outline_color = ImColor(33, 35, 40, 255);
ImColor menu_background_color = ImColor(45, 48, 55, 255);
ImColor menu_gradient_left_color = ImColor(200, 110, 45, 255);
ImColor menu_gradient_right_color = ImColor(255, 185, 100, 255);
ImColor menu_separator_color = ImColor(62, 68, 78, 255);
ImColor menu_active_tab_color = ImColor(55, 60, 69, 255);
ImColor menu_hovered_tab_color = ImColor(50, 55, 62, 255);
ImColor menu_checkbox_color = ImColor(85, 97, 119, 255);
ImColor menu_checkbox_hovered_color = ImColor(55, 60, 69, 255);
ImColor menu_slider_background_color = ImColor(33, 35, 40, 255);
ImColor menu_slider_value_background_color = ImColor(85, 97, 119, 255);
ImColor menu_combobox_outline_color = ImColor(85, 97, 119, 255);
ImColor menu_combobox_background_color = ImColor(59, 63, 72, 255);
ImColor menu_combobox_hovered_color = ImColor(45, 48, 55, 255);
ImColor menu_combobox_arrow_color = ImColor(224, 230, 240, 255);
ImColor menu_button_outline_color = ImColor(85, 97, 119, 255);
ImColor menu_button_background_color = ImColor(59, 63, 72, 255);
ImColor menu_button_hovered_color = ImColor(79, 84, 95, 255);
ImColor menu_keybind_outline_color = ImColor(85, 97, 119, 255);
ImColor menu_keybind_background_color = ImColor(51, 54, 63, 255);
ImColor menu_keybind_opened_color = ImColor(55, 60, 69, 255);
ImColor menu_keybind_hovered_color = ImColor(59, 63, 72, 255);
ImColor menu_groupbox_color = ImColor(50, 55, 62, 255);
ImColor menu_first_text_color = ImColor(214, 217, 224, 255);
ImColor menu_second_text_color = ImColor(189, 198, 219, 255);
ImColor menu_third_text_color = ImColor(218, 225, 243, 255);

// Enums and structs from onetap_menu.cpp
enum KEYBIND_TYPES {
    HOLD,
    TOGGLE,
    ALWAYS_ON
};

struct menu_tooltip_element {
    const char* title;
    const char* tooltip;
    bool hovered;
};

struct key_bind {
    int key;
    int type;
};

// Global variables from onetap_menu.cpp
const char* menu_title = "Mozority | Beta Test"; // Adapted to match menu.cpp
int elements_count = 0;
ImVec2 rendering_tooltip_pos = ImVec2(0, 0);
bool rendering_tooltip = false;
int hovering_tooltip_time = 0;
int hovering_tooltip_index = -1;
int active_element = -1;
int active_element_type = 0;
float element_font_size = 14.0f;
std::vector<menu_tooltip_element> tooltips;
ImVec2 menu_position = ImVec2(50, 50); // Matches menu.cpp default
ImVec2 menu_size = ImVec2(700, 500);  // Matches menu.cpp default
ImVec2 mouse_position = ImVec2(0, 0);

enum ELEMENTS {
    NONE,
    CHECKBOX,
    SLIDER_INT,
    SLIDER_FLOAT,
    COMBOBOX,
    MULTI_COMBOBOX,
    KEYBIND,
    BUTTON,
};

std::vector<const char*> menu_keybind_modes = { "hold", "toggle", "always on" };

// Custom UI classes from onetap_menu.cpp (simplified for integration)
class groupbox {
public:
    groupbox(const char* tit, ImVec2 pos, ImVec2 siz) {
        position = pos;
        size = siz;
        element_position = ImVec2(pos.x + 9, pos.y + 19);
        element_count = 0;
        previous_element = 0;
        title = tit;
    }

    void draw(ImVec2 pos) {
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        position = pos;
        ImVec2 groupbox_main_rect_min = position;
        ImVec2 groupbox_main_rect_max = ImVec2(position.x + size.x, position.y + size.y);
        ImVec2 groupbox_gradient_rect_min = position;
        ImVec2 groupbox_gradient_rect_max = ImVec2(position.x + size.x, position.y + 6);
        ImVec2 groupbox_text_position = ImVec2(position.x + 3, position.y - 10 - segoe_ui_bold->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, title).y * 0.7f);

        draw_list->AddText(segoe_ui_bold, 16.0f, groupbox_text_position, menu_gradient_right_color, title);
        draw_list->AddRectFilled(groupbox_main_rect_min, groupbox_main_rect_max, menu_groupbox_color, 3.0f);
        draw_list->AddRectFilledMultiColor(groupbox_gradient_rect_min, groupbox_gradient_rect_max, menu_gradient_left_color, menu_gradient_right_color, menu_gradient_right_color, menu_gradient_left_color);
    }

    void end() {
        if (element_count > 0)
            size = ImVec2(size.x, (element_position.y - position.y - 12) + 14);
        element_position = ImVec2(position.x + 9, position.y + 19);
        element_count = 0;
        previous_element = 0;
    }

    ImVec2 position;
    ImVec2 size;
    ImVec2 element_position;
    int element_count;
    int previous_element;
    const char* title;
};

// Utility functions from onetap_menu.cpp
void draw_tooltips() {
    int hovered = 0;
    int last_hovered = -1;

    if (tooltips.empty())
        return;

    for (int i = 0; i < tooltips.size(); i++) {
        if (std::string(tooltips.at(i).tooltip) == "none")
            continue;

        if (hovering_tooltip_index == -1 && tooltips.at(i).hovered) {
            hovering_tooltip_index = i;
            hovering_tooltip_time++;
            hovered++;
            last_hovered = i;
        }
    }

    if (hovering_tooltip_index != -1) {
        if (tooltips.at(hovering_tooltip_index).hovered)
            hovering_tooltip_time++;
        else {
            rendering_tooltip_pos = ImVec2(0, 0);
            rendering_tooltip = false;
            hovering_tooltip_index = -1;
            hovering_tooltip_time = 0;
        }
    }

    if (hovered > 0 && last_hovered > -1) {
        if (hovered > 1)
            hovering_tooltip_index = last_hovered;
    }

    if (hovering_tooltip_index == -1)
        return;

    if (hovering_tooltip_time > 50 && tooltips.at(hovering_tooltip_index).hovered) {
        if (!rendering_tooltip) {
            rendering_tooltip_pos = ImVec2(ImGui::GetMousePos().x + 20, ImGui::GetMousePos().y);
            rendering_tooltip = true;
        }

        if (rendering_tooltip) {
            if (int(ImGui::GetMousePos().x) != int(rendering_tooltip_pos.x) && int(ImGui::GetMousePos().y) != int(rendering_tooltip_pos.y)) {
                hovering_tooltip_index = -1;
                hovering_tooltip_time = 0;
                rendering_tooltip_pos = ImVec2(0, 0);
                rendering_tooltip = false;
                return;
            }

            ImGui::GetForegroundDrawList()->AddRectFilled(
                rendering_tooltip_pos,
                ImVec2(rendering_tooltip_pos.x + 10 + segoe_ui_semibold->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, tooltips.at(hovering_tooltip_index).tooltip).x + 10, rendering_tooltip_pos.y + 25),
                menu_slider_background_color,
                3.0f
            );
            ImGui::GetForegroundDrawList()->AddText(
                segoe_ui_semibold,
                14.0f,
                ImVec2(rendering_tooltip_pos.x + 10, rendering_tooltip_pos.y + 11 - segoe_ui_semibold->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, tooltips.at(hovering_tooltip_index).tooltip).y * 0.5f),
                menu_second_text_color,
                tooltips.at(hovering_tooltip_index).tooltip
            );
        }
    }

    if (!tooltips.at(hovering_tooltip_index).hovered) {
        hovering_tooltip_index = -1;
        hovering_tooltip_time = 0;
        rendering_tooltip_pos = ImVec2(0, 0);
        rendering_tooltip = false;
    }
}

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

    if (ImGui::ColorButton(label, ImVec4(col[0], col[1], col[2], col[3]), ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip)) {
        ImGui::OpenPopup(label);
    }

    if (ImGui::BeginPopup(label)) {
        ImGui::PushItemWidth(200);
        ImGui::ColorPicker4("##picker", col, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar);
        ImGui::PopItemWidth();
        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(3);
}

void ApplyImGuiTheme() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Adapted colors from onetap_menu.cpp
    colors[ImGuiCol_WindowBg] = ImVec4(45 / 255.0f, 48 / 255.0f, 55 / 255.0f, 1.0f); // menu_background_color
    colors[ImGuiCol_ChildBg] = ImVec4(50 / 255.0f, 55 / 255.0f, 62 / 255.0f, 1.0f);   // menu_groupbox_color
    colors[ImGuiCol_PopupBg] = ImVec4(59 / 255.0f, 63 / 255.0f, 72 / 255.0f, 1.0f);  // menu_combobox_background_color

    colors[ImGuiCol_FrameBg] = ImVec4(33 / 255.0f, 35 / 255.0f, 40 / 255.0f, 1.0f);        // menu_slider_background_color
    colors[ImGuiCol_FrameBgHovered] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f); // menu_checkbox_hovered_color
    colors[ImGuiCol_FrameBgActive] = ImVec4(85 / 255.0f, 97 / 255.0f, 119 / 255.0f, 1.0f); // menu_checkbox_color

    colors[ImGuiCol_Text] = ImVec4(214 / 255.0f, 217 / 255.0f, 224 / 255.0f, 1.0f); // menu_first_text_color
    colors[ImGuiCol_TextDisabled] = ImVec4(189 / 255.0f, 198 / 255.0f, 219 / 255.0f, 1.0f); // menu_second_text_color
    colors[ImGuiCol_TextSelectedBg] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);  // menu_active_tab_color

    colors[ImGuiCol_TitleBg] = ImVec4(45 / 255.0f, 48 / 255.0f, 55 / 255.0f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(45 / 255.0f, 48 / 255.0f, 55 / 255.0f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(45 / 255.0f, 48 / 255.0f, 55 / 255.0f, 1.0f);

    colors[ImGuiCol_Button] = ImVec4(59 / 255.0f, 63 / 255.0f, 72 / 255.0f, 1.0f);    // menu_button_background_color
    colors[ImGuiCol_ButtonHovered] = ImVec4(79 / 255.0f, 84 / 255.0f, 95 / 255.0f, 1.0f); // menu_button_hovered_color
    colors[ImGuiCol_ButtonActive] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);  // menu_active_tab_color

    colors[ImGuiCol_Tab] = ImVec4(45 / 255.0f, 48 / 255.0f, 55 / 255.0f, 1.0f);         // menu_background_color
    colors[ImGuiCol_TabHovered] = ImVec4(50 / 255.0f, 55 / 255.0f, 62 / 255.0f, 1.0f);  // menu_hovered_tab_color
    colors[ImGuiCol_TabActive] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);   // menu_active_tab_color
    colors[ImGuiCol_TabUnfocused] = ImVec4(45 / 255.0f, 48 / 255.0f, 55 / 255.0f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);

    colors[ImGuiCol_SliderGrab] = ImVec4(85 / 255.0f, 97 / 255.0f, 119 / 255.0f, 1.0f);  // menu_slider_value_background_color
    colors[ImGuiCol_SliderGrabActive] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);

    colors[ImGuiCol_CheckMark] = ImVec4(85 / 255.0f, 97 / 255.0f, 119 / 255.0f, 1.0f); // menu_checkbox_color

    colors[ImGuiCol_Header] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(50 / 255.0f, 55 / 255.0f, 62 / 255.0f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);

    colors[ImGuiCol_Separator] = ImVec4(62 / 255.0f, 68 / 255.0f, 78 / 255.0f, 1.0f); // menu_separator_color
    colors[ImGuiCol_SeparatorHovered] = ImVec4(50 / 255.0f, 55 / 255.0f, 62 / 255.0f, 1.0f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);

    colors[ImGuiCol_Border] = ImVec4(33 / 255.0f, 35 / 255.0f, 40 / 255.0f, 1.0f); // menu_background_outline_color
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    colors[ImGuiCol_ScrollbarBg] = ImVec4(45 / 255.0f, 48 / 255.0f, 55 / 255.0f, 1.0f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(50 / 255.0f, 55 / 255.0f, 62 / 255.0f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);

    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(33 / 255.0f, 35 / 255.0f, 40 / 255.0f, 0.35f);
    colors[ImGuiCol_NavHighlight] = ImVec4(50 / 255.0f, 55 / 255.0f, 62 / 255.0f, 1.0f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f);

    style.WindowRounding = 3.0f; // Adjusted to match onetap_menu.cpp style
    style.ChildRounding = 3.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;

    style.WindowBorderSize = 1.0f;
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

    // Load fonts from onetap_menu.cpp
    segoe_ui_semibold = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 14.0f);
    segoe_ui_bold = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f);
    io.FontDefault = segoe_ui_semibold; // Set default font
    smallFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 12.0f);
    largeFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 24.0f);

    std::cout << "initialized menu\n";
}

void Menu::render() {
    keybind.pollInputs();
    if (!showMenu) return;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowSize(menu_size, ImGuiCond_Once);
    ImGui::SetNextWindowPos(menu_position, ImGuiCond_Once);

    ImGui::Begin(menu_title, nullptr, window_flags);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
    ImGui::BeginChild("Sidebar", ImVec2(48, 0), true, ImGuiWindowFlags_NoScrollbar);

    ImGui::PushFont(largeFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("MZ").x) * 0.5f);
    ImGui::Text("MZ");
    ImGui::PopFont();

    ImGui::PushFont(smallFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Beta").x) * 0.5f);
    ImGui::TextDisabled("Beta");
    ImGui::PopFont();
    ImGui::Separator();

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
            tooltips.push_back({ tabNames[i], tabNames[i], true });
        }
        else {
            tooltips.push_back({ tabNames[i], tabNames[i], false });
        }
        if (isSelected) {
            ImGui::PopStyleColor();
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetTextLineHeight() - ImGui::GetStyle().WindowPadding.y);
    ImGui::PushFont(smallFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(">.<").x) * 0.5f);
    ImGui::TextDisabled(">.<");
    ImGui::PopFont();

    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::SameLine();

    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(59 / 255.0f, 63 / 255.0f, 72 / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(85 / 255.0f, 97 / 255.0f, 119 / 255.0f, 1.0f));

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

        if (aimSubTab == 0) {
            ImGui::Text("Aimbot Settings");
            ImGui::Separator();

            ImGui::BeginChild("LeftSection", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 10, 0), true);
            ImGui::BeginGroup();
            if (ImGui::Checkbox("Enable##AimBot", &Config::aimbot) && ImGui::IsItemHovered()) {
                tooltips.push_back({ "Enable Aimbot", "Toggles the aimbot feature", true });
            }
            else {
                tooltips.push_back({ "Enable Aimbot", "Toggles the aimbot feature", false });
            }
            ImGui::SameLine();
            ImGui::Text("Key:");
            ImGui::SameLine();
            keybind.menuButton(Config::aimbot, false);
            Keybind* aimbotKeybind = keybind.getKeybind(Config::aimbot);
            if (aimbotKeybind) {
                ImGui::PushItemWidth(80);
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
            ImGui::Checkbox("Wall Check", &Config::aimbot_wall_check);
            ImGui::SliderFloat("Smooth", &Config::aimbot_smoothness, 0.0f, 30.0f, "%.1f");
            ImGui::PopItemWidth();
            ImGui::EndGroup();
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("RightSection", ImVec2(0, 0), true);
            ImGui::BeginGroup();
            //ImGui::Checkbox("Wall Check", &Config::aimbot_wall_check);
            //ImGui::SameLine();
            //ImGui::TextDisabled("(!)");
            ImGui::Checkbox("Draw FOV Circle", &Config::fov_circle);
            if (Config::fov_circle) {
                ImGui::SameLine(0, 10);
                CustomColorPicker("Circle Color##FovColor", (float*)&Config::fovCircleColor);
            }
            ImGui::Checkbox("Multiple Bones", &Config::aimbot_multipoint);
            ImGui::PushItemWidth(180);
            ImGui::SliderInt("Bones Count", &Config::aimbot_multipoint_count, 1, 20, "%d");
            ImGui::SliderFloat("Max Aim Dist.", &Config::aimbot_max_distance, 250.0f, 10000.0f, "%.0f units");
            ImGui::PopItemWidth();
            ImGui::EndGroup();
            ImGui::EndChild();
        }
        else if (aimSubTab == 1) {
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

        if (visualsSubTab == 0) {
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
        else if (visualsSubTab == 1) {
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
        else if (visualsSubTab == 2) {
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
        else if (visualsSubTab == 3) {
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
        else if (visualsSubTab == 4) {
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

        if (miscSubTab == 0) {
            ImGui::Text("No additional settings");
        }
        else if (miscSubTab == 1) {
            ImGui::BeginGroup();
            ImGui::Text("Other Settings");
            ImGui::Separator();
            ImGui::Checkbox("Show Keybinds", &Config::show_keybinds);
            ImGui::Checkbox("Show Watermark", &Config::show_watermark);
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

    draw_tooltips();
    elements_count = 0;
    tooltips.clear();
}

void Menu::toggleMenu() {
    showMenu = !showMenu;
}