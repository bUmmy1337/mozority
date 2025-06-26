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
ImColor menu_gradient_left_color = ImColor(230, 230, 230, 255);
ImColor menu_gradient_right_color = ImColor(218, 218, 218, 255);
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
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Темная палитра с бело-антарктическим акцентом
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.09f, 0.10f, 1.0f); // Очень темно-серый
    colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.11f, 0.12f, 1.0f);   // Чуть светлее
    colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.13f, 0.14f, 1.0f);  // Еще чуть светлее

    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.0f);        // Темно-серый фон элементов
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.21f, 0.22f, 1.0f); // Наведенный фон
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f); // Активный фон (приглушенный голубой акцент)

    colors[ImGuiCol_Text] = ImVec4(0.90f, 0.92f, 0.94f, 1.0f); // Светлый текст
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.65f, 0.70f, 1.0f); // Средне-серый текст
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.30f, 0.40f, 0.50f, 0.4f);  // Фон выделенного текста

    colors[ImGuiCol_TitleBg] = colors[ImGuiCol_WindowBg];
    colors[ImGuiCol_TitleBgActive] = colors[ImGuiCol_WindowBg];
    colors[ImGuiCol_TitleBgCollapsed] = colors[ImGuiCol_WindowBg];

    colors[ImGuiCol_Button] = ImVec4(0.15f, 0.16f, 0.17f, 1.0f);    // Темно-серые кнопки
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.21f, 0.22f, 1.0f); // Наведенные кнопки
    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);  // Активные кнопки

    colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.11f, 0.12f, 1.0f);         // Фон табов
    colors[ImGuiCol_TabHovered] = ImVec4(0.15f, 0.16f, 0.17f, 1.0f);  // Наведенный таб
    colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);   // Активный таб (акцент)
    colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_Tab];
    colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabActive];

    colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);  // Акцентный цвет для ползунков
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.50f, 0.60f, 1.0f); // Более насыщенный акцент

    colors[ImGuiCol_CheckMark] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f); // Акцентный цвет для чекбоксов

    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.16f, 0.17f, 1.0f); // Заголовки
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.21f, 0.22f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);

    colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.16f, 0.17f, 1.0f); // Разделители
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.20f, 0.21f, 0.22f, 1.0f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);

    colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Убираем рамки
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.11f, 0.12f, 1.0f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.21f, 0.22f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.26f, 0.27f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);

    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.06f, 0.07f, 0.60f); // Более темный фон модальных окон
    colors[ImGuiCol_NavHighlight] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.30f, 0.40f, 0.50f, 1.0f);

    // Стилизация (оставляем те же значения для скругления и отступов, что и в предыдущем варианте, так как они хорошо подходят для современного вида)
    style.WindowRounding = 10.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 8.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 8.0f;
    style.TabRounding = 8.0f;

    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.PopupBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;

    style.ItemSpacing = ImVec2(15, 12);
    style.ItemInnerSpacing = ImVec2(10, 10);
    style.FramePadding = ImVec2(12, 10);
    style.WindowPadding = ImVec2(20, 20);

    style.GrabMinSize = 20.0f;
    style.ScrollbarSize = 16.0f;
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

void Menu::handleInsertKeyToggle() {
    bool currentState = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;

    if (currentState && !insertKeyLastState) {
        if (!insertKeyPressed) {
            this->toggleMenu();
            insertKeyPressed = true;
        }
    }
    else if (!currentState) {
        insertKeyPressed = false;
    }

    insertKeyLastState = currentState;
}

void Menu::render() {
    keybind.pollInputs();
    if (!showMenu) return;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowSize(menu_size, ImGuiCond_Once);
    ImGui::SetNextWindowPos(menu_position, ImGuiCond_Once);

    ImGui::Begin(menu_title, nullptr, window_flags);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
    // Sidebar
    ImGui::BeginChild("Sidebar", ImVec2(100, 0), true, ImGuiWindowFlags_NoScrollbar);

    ImGui::PushFont(largeFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("mozority").x) * 0.5f);
    ImGui::TextColored(menu_gradient_right_color, "mozority");
    ImGui::PopFont();

    ImGui::PushFont(smallFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("open beta").x) * 0.5f);
    ImGui::TextColored(menu_second_text_color, "open beta");
    ImGui::PopFont();
    ImGui::Separator();

    // Основные вкладки
    const char* mainTabs[] = { "Aim", "Visuals", "Misc", "Config" };
    for (int i = 0; i < 4; i++) {
        std::string tabText = (activeTab == i) ? ">> " + std::string(mainTabs[i]) : "> " + std::string(mainTabs[i]);
        ImVec2 posBefore = ImGui::GetCursorScreenPos();
        if (ImGui::Selectable(tabText.c_str(), activeTab == i, 0, ImVec2(0, 20))) {
            activeTab = i;
        }
        if (activeTab == i) {
            ImVec2 textSize = ImGui::CalcTextSize(tabText.c_str());
            ImGui::GetWindowDrawList()->AddLine(
                ImVec2(posBefore.x, posBefore.y + 18),
                ImVec2(posBefore.x + textSize.x, posBefore.y + 18),
                IM_COL32(255, 255, 255, 255),
                2.0f
            );
        }
        if (activeTab == i) {
            ImGui::Indent(10.0f);
            if (i == 0) { // Aim
                const char* subTabs[] = { "Aimbot", "TriggerBot" };
                for (int j = 0; j < 2; j++) {
                    std::string subTabText = (aimSubTab == j) ? ">> " + std::string(subTabs[j]) : "> " + std::string(subTabs[j]);
                    ImVec2 subPosBefore = ImGui::GetCursorScreenPos();
                    if (ImGui::Selectable(subTabText.c_str(), aimSubTab == j, 0, ImVec2(0, 20))) {
                        aimSubTab = j;
                    }
                    if (aimSubTab == j) {
                        ImVec2 subTextSize = ImGui::CalcTextSize(subTabText.c_str());
                        ImGui::GetWindowDrawList()->AddLine(
                            ImVec2(subPosBefore.x, subPosBefore.y + 18),
                            ImVec2(subPosBefore.x + subTextSize.x, subPosBefore.y + 18),
                            IM_COL32(255, 255, 255, 255),
                            2.0f
                        );
                    }
                }
            }
            else if (i == 1) { // Visuals
                const char* subTabs[] = { "ESP", "World", "Chams", "Local", "Removals" };
                for (int j = 0; j < 5; j++) {
                    std::string subTabText = (visualsSubTab == j) ? ">> " + std::string(subTabs[j]) : "> " + std::string(subTabs[j]);
                    ImVec2 subPosBefore = ImGui::GetCursorScreenPos();
                    if (ImGui::Selectable(subTabText.c_str(), visualsSubTab == j, 0, ImVec2(0, 20))) {
                        visualsSubTab = j;
                    }
                    if (visualsSubTab == j) {
                        ImVec2 subTextSize = ImGui::CalcTextSize(subTabText.c_str());
                        ImGui::GetWindowDrawList()->AddLine(
                            ImVec2(subPosBefore.x, subPosBefore.y + 18),
                            ImVec2(subPosBefore.x + subTextSize.x, subPosBefore.y + 18),
                            IM_COL32(255, 255, 255, 255),
                            2.0f
                        );
                    }
                }
            }
            else if (i == 2) { // Misc
                const char* subTabs[] = { "Movement", "Other" };
                for (int j = 0; j < 2; j++) {
                    std::string subTabText = (miscSubTab == j) ? ">> " + std::string(subTabs[j]) : "> " + std::string(subTabs[j]);
                    ImVec2 subPosBefore = ImGui::GetCursorScreenPos();
                    if (ImGui::Selectable(subTabText.c_str(), miscSubTab == j, 0, ImVec2(0, 20))) {
                        miscSubTab = j;
                    }
                    if (miscSubTab == j) {
                        ImVec2 subTextSize = ImGui::CalcTextSize(subTabText.c_str());
                        ImGui::GetWindowDrawList()->AddLine(
                            ImVec2(subPosBefore.x, subPosBefore.y + 18),
                            ImVec2(subPosBefore.x + subTextSize.x, subPosBefore.y + 18),
                            IM_COL32(255, 255, 255, 255),
                            2.0f
                        );
                    }
                }
            }
            ImGui::Unindent(10.0f);
        }
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetTextLineHeight() - ImGui::GetStyle().WindowPadding.y);
    ImGui::PushFont(smallFont);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(">.<").x) * 0.5f);
    ImGui::TextColored(menu_third_text_color, ">.<");
    ImGui::PopFont();

    ImGui::EndChild();

    ImGui::PopStyleVar();

    ImGui::SameLine();

    // Основное содержимое
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(59 / 255.0f, 63 / 255.0f, 72 / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(55 / 255.0f, 60 / 255.0f, 69 / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(85 / 255.0f, 97 / 255.0f, 119 / 255.0f, 1.0f));

    switch (activeTab) {
    case 0: // Aim
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
            ImGui::Text("TriggerBot Settings");
            ImGui::Separator();
            ImGui::Text("No additional settings");
        }
        break;
    case 1: // Visuals
        if (visualsSubTab == 0) {
            ImGui::BeginGroup();
            ImGui::Text("ESP Settings");
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
            ImGui::Text("Local Settings");
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
    case 2: // Misc
        if (miscSubTab == 0) {
            ImGui::Text("Movement Settings");
            ImGui::Separator();
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
    case 3: // Config
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