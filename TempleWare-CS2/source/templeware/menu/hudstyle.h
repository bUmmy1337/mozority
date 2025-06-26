#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx11.h"
#include "../../../external/imgui/imgui_impl_win32.h"
ImColor mmenu_background_outline_color = ImColor(20, 22, 24, 255); // ����� �����-�����
ImColor mmenu_background_color = ImColor(25, 27, 30, 255);       // ����� ������
ImColor mmenu_gradient_left_color = ImColor(180, 220, 255, 255);    // ������� ������� (����-�������������� ������)
ImColor mmenu_gradient_right_color = ImColor(255, 255, 255, 255);   // ������ ����� (����-�������������� ������)
ImColor mmenu_separator_color = ImColor(40, 45, 50, 255);        // ������ �����������
ImColor mmenu_active_tab_color = ImColor(45, 50, 55, 255);       // �������� ��� (������)
ImColor mmenu_hovered_tab_color = ImColor(35, 40, 45, 255);      // ���������� ��� (������)
ImColor mmenu_checkbox_color = ImColor(80, 90, 100, 255);         // ���� �������� (������)
ImColor mmenu_checkbox_hovered_color = ImColor(60, 70, 80, 255); // ���������� ������� (������)
ImColor mmenu_slider_background_color = ImColor(30, 32, 35, 255); // ��� �������� (������)
ImColor mmenu_slider_value_background_color = ImColor(100, 140, 180, 255); // �������� �������� (��������� �������)
ImColor mmenu_combobox_outline_color = ImColor(80, 90, 100, 255);
ImColor mmenu_combobox_background_color = ImColor(35, 40, 45, 255);
ImColor mmenu_combobox_hovered_color = ImColor(45, 50, 55, 255);
ImColor mmenu_combobox_arrow_color = ImColor(200, 210, 220, 255);     // ������� �������
ImColor mmenu_button_outline_color = ImColor(80, 90, 100, 255);
ImColor mmenu_button_background_color = ImColor(35, 40, 45, 255);
ImColor mmenu_button_hovered_color = ImColor(45, 50, 55, 255);
ImColor mmenu_keybind_outline_color = ImColor(80, 90, 100, 255);
ImColor mmenu_keybind_background_color = ImColor(30, 32, 35, 255);
ImColor mmenu_keybind_opened_color = ImColor(45, 50, 55, 255);
ImColor mmenu_keybind_hovered_color = ImColor(35, 40, 45, 255);
ImColor mmenu_groupbox_color = ImColor(30, 32, 35, 255);         // ��� ���������� (������)
ImColor mmenu_first_text_color = ImColor(230, 235, 240, 255);          // ������� �����
ImColor mmenu_second_text_color = ImColor(180, 190, 200, 255);        // ������� �����
ImColor mmenu_third_text_color = ImColor(120, 130, 140, 255);       // ����� ������ ����� �����
extern ImFont* segoe_ui_semibold;
extern ImFont* segoe_ui_bold;