#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_dx11.h"
#include "../../../external/imgui/imgui_impl_win32.h"
#include "../keybinds/keybinds.h"


class Menu {
public:
    Menu();
    ~Menu();
    void init(HWND& window, ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ID3D11RenderTargetView* mainRenderTargetView);
    void render();
    void toggleMenu();

private:
    bool showMenu;
    int activeTab;
    int aimSubTab;
    int visualsSubTab;
    int miscSubTab;
    ImFont* smallFont;
    Keybinds& keybind; // Reference to global Keybinds instance

    // Texture-related members
    ID3D11ShaderResourceView* tabIconsTextures[4]; // Textures for tab icons (Aimbot, Visuals, Misc, Config)
    ID3D11ShaderResourceView* logoTexture; // Texture for top logo
    ImVec2 iconSize; // Size of tab icons
    ImVec2 logoSize; // Size of logo icon

    void ApplyImGuiTheme();
    bool LoadTextureFromMemory(unsigned char* data, size_t dataSize, ID3D11Device* device, ID3D11ShaderResourceView** outSrv, int* outWidth, int* outHeight);
};
