#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx11.h"
#include "../../../external/imgui/imgui_impl_win32.h"


class Menu {
public:
	Menu();

	void init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView);
	void render();

	void toggleMenu();
	bool& getShowMenu() { return showMenu; }
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* mainRenderTargetView = nullptr;
	void handleInsertKeyToggle(); // Объявление метода
private:
	bool showMenu;
	int activeTab;
	int aimSubTab;      // Added for Aim sub-functions
	int visualsSubTab;  // Added for Visuals sub-functions
	int miscSubTab;     // Added for Misc sub-functions
	ImFont* smallFont;
	ImFont* largeFont;
	ImFont* iconFont;
	bool insertKeyPressed = false;
	bool insertKeyLastState = false;
};
