#include "debug/debug.h"

#include "includes.h"
#include "templeware/templeware.h"
#include "templeware/renderer/icons.h"

#include "../external/kiero/minhook/include/MinHook.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

TempleWare templeWare;

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
        return true;
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
#ifdef TEMPLEDEBUG
            initDebug();
#endif


            templeWare.init(window, pDevice, pContext, mainRenderTargetView);
            init = true;
        }
        else
            return oPresent(pSwapChain, SyncInterval, Flags);
    }

    ImFontConfig imIconsConfig;
    imIconsConfig.RasterizerMultiply = 1.2f;

    constexpr ImWchar wIconRanges[] =
    {
        0xE000, 0xF8FF, // Private Use Area
        0
    };

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    templeWare.renderer.menu.handleInsertKeyToggle(); // new menu open logic

    templeWare.renderer.menu.render();
    templeWare.renderer.hud.render();

    // Always call esp() to allow individual components to be rendered
    templeWare.renderer.visuals.esp();

    ImGui::Render();
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

void init_console() {
    if (::AllocConsole()) {
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);  // Redirect stdout to console
        freopen_s(&f, "CONOUT$", "w", stderr);  // Redirect stderr to console
        ::SetConsoleTitleW(L"mozority.pro");

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf(R"(                                                d8b 888             
                                                Y8P 888             
                                                    888             
88888b.d88b.   .d88b. 88888888  .d88b.  888d888 888 888888 888  888 
888 "888 "88b d88""88b   d88P  d88""88b 888P"   888 888    888  888 
888  888  888 888  888  d88P   888  888 888     888 888    888  888 
888  888  888 Y88..88P d88P    Y88..88P 888     888 Y88b.  Y88b 888 
888  888  888  "Y88P" 88888888  "Y88P"  888     888  "Y888  "Y88888 
                                                                888 
                                                           Y8b d88P 
                                                            "Y88P"  
)");

        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        printf("[");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("+");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("] mozority.pro\n");
    }
}
DWORD WINAPI MainThread(LPVOID lpReserved)
{
    bool init_hook = false;
    do
    {
        init_console();

        // hook hkPresent and init cheat
        if (!init_hook) {
            if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
            {
                kiero::bind(8, (void**)&oPresent, hkPresent);
                init_hook = true;

                // hide console
                 if (const HWND hConsoleWindow = ::GetConsoleWindow(); hConsoleWindow != nullptr)
                     ::ShowWindow(hConsoleWindow, SW_HIDE);
            }
        }
    } while (!GetAsyncKeyState(VK_F4));

    if (oWndProc != nullptr)
    {
        // restore wnd proc
        SetWindowLongPtrW(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));

        // invalidate old wnd proc
        oWndProc = nullptr;
    }

    kiero::shutdown();

    // destroy minhook
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    // free allocated memory for console
    ::FreeConsole();

    // close console window
    if (const HWND hConsoleWindow = ::GetConsoleWindow(); hConsoleWindow != nullptr)
        ::PostMessageW(hConsoleWindow, WM_CLOSE, 0U, 0L);

    fclose(stdout);
    fclose(stderr);

    // close thread
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpReserved), EXIT_SUCCESS);

    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        kiero::shutdown();
        break;
    }
    return TRUE;
}
