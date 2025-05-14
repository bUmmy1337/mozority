#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx11.h"
#include "../../../external/imgui/imgui_impl_win32.h"
ImColor mmenu_background_outline_color = ImColor(33, 35, 40, 255);
ImColor mmenu_background_color = ImColor(45, 48, 55, 255);
ImColor mmenu_gradient_left_color = ImColor(45, 110, 200, 255); // Changed to blue
ImColor mmenu_gradient_right_color = ImColor(100, 185, 255, 255); // Changed to blue
ImColor mmenu_separator_color = ImColor(62, 68, 78, 255);
ImColor mmenu_active_tab_color = ImColor(55, 60, 69, 255);
ImColor mmenu_hovered_tab_color = ImColor(50, 55, 62, 255);
ImColor mmenu_checkbox_color = ImColor(85, 97, 119, 255);
ImColor mmenu_checkbox_hovered_color = ImColor(55, 60, 69, 255);
ImColor mmenu_slider_background_color = ImColor(33, 35, 40, 255);
ImColor mmenu_slider_value_background_color = ImColor(85, 97, 119, 255);
ImColor mmenu_combobox_outline_color = ImColor(85, 97, 119, 255);
ImColor mmenu_combobox_background_color = ImColor(59, 63, 72, 255);
ImColor mmenu_combobox_hovered_color = ImColor(45, 48, 55, 255);
ImColor mmenu_combobox_arrow_color = ImColor(224, 230, 240, 255);
ImColor mmenu_button_outline_color = ImColor(85, 97, 119, 255);
ImColor mmenu_button_background_color = ImColor(59, 63, 72, 255);
ImColor mmenu_button_hovered_color = ImColor(79, 84, 95, 255);
ImColor mmenu_keybind_outline_color = ImColor(85, 97, 119, 255);
ImColor mmenu_keybind_background_color = ImColor(51, 54, 63, 255);
ImColor mmenu_keybind_opened_color = ImColor(55, 60, 69, 255);
ImColor mmenu_keybind_hovered_color = ImColor(59, 63, 72, 255);
ImColor mmenu_groupbox_color = ImColor(50, 55, 62, 255);
ImColor mmenu_first_text_color = ImColor(214, 217, 224, 255);
ImColor mmenu_second_text_color = ImColor(189, 198, 219, 255);
ImColor mmenu_third_text_color = ImColor(218, 225, 243, 255);
extern ImFont* segoe_ui_semibold;
extern ImFont* segoe_ui_bold;