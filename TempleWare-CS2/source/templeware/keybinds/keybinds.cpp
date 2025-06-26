#include "keybinds.h"
#include <Windows.h>
#include <cstring>   // For strcpy_s and sprintf_s
#include <cstdio>    // For sprintf_s

#include "../../../external/imgui/imgui.h"
#include "../config/config.h"

Keybind::Keybind(bool& v, int k, KeybindMode m)
    : var(v), key(k), mode(m), isListening(false), skipFrame(false) {
}

Keybinds::Keybinds() {
    keybinds.emplace_back(Keybind(Config::aimbot, VK_XBUTTON1, KeybindMode::Toggle));
}

void Keybinds::pollInputs() {
    for (Keybind& k : keybinds) {
        if (k.key != 0) {
            if (k.mode == KeybindMode::Toggle) {
                // Замена на GetKeyState с проверкой предыдущего состояния
                static bool prevState[256] = { false };
                bool currentState = (GetKeyState(k.key) & 0x8000) != 0;
                if (currentState && !prevState[k.key]) {
                    k.var = !k.var;
                }
                prevState[k.key] = currentState;
            }
            else if (k.mode == KeybindMode::Hold) {
                k.var = (GetKeyState(k.key) & 0x8000) != 0;
            }
        }
    }
}

void Keybinds::menuButton(bool& var, bool renderCombo) {
    for (auto& kb : keybinds) {
        if (&kb.var != &var) continue;

        char keyName[32] = "None";
        if (kb.key != 0) {
            switch (kb.key) {
            case VK_INSERT: strcpy_s(keyName, "INSERT"); break;
            case VK_DELETE: strcpy_s(keyName, "DELETE"); break;
            case VK_HOME: strcpy_s(keyName, "HOME"); break;
            case VK_END: strcpy_s(keyName, "END"); break;
            case VK_PRIOR: strcpy_s(keyName, "PAGE UP"); break;
            case VK_NEXT: strcpy_s(keyName, "PAGE DOWN"); break;
            case VK_LBUTTON: strcpy_s(keyName, "MOUSE1"); break;
            case VK_RBUTTON: strcpy_s(keyName, "MOUSE2"); break;
            case VK_MBUTTON: strcpy_s(keyName, "MOUSE3"); break;
            case VK_XBUTTON1: strcpy_s(keyName, "MOUSE4"); break;
            case VK_XBUTTON2: strcpy_s(keyName, "MOUSE5"); break;
            default:
                if (kb.key >= 'A' && kb.key <= 'Z') {
                    sprintf_s(keyName, "%c", kb.key);
                }
                else if (kb.key >= '0' && kb.key <= '9') {
                    sprintf_s(keyName, "%c", kb.key);
                }
                else {
                    sprintf_s(keyName, "0x%X", kb.key);
                }
                break;
            }
        }

        if (!kb.isListening) {
            ImGui::PushID(&kb);
            ImGui::Text("[%s]", keyName);
            ImGui::SameLine();
            bool clicked = ImGui::Button("Bind##Bind", ImVec2(60, 0));
            if (renderCombo) {
                ImGui::SameLine();
                const char* modes[] = { "Toggle", "Hold" };
                int current_mode = static_cast<int>(kb.mode);
                ImGui::PushItemWidth(60);
                if (ImGui::Combo("Mode", &current_mode, modes, IM_ARRAYSIZE(modes))) {
                    kb.mode = static_cast<KeybindMode>(current_mode);
                }
                ImGui::PopItemWidth();
            }
            ImGui::PopID();

            if (clicked) {
                kb.isListening = true;
                kb.skipFrame = true;
            }
        }
        else {
            ImGui::Text("Press any key...");
            ImGui::SameLine();

            if (ImGui::Button("Cancel") || (GetAsyncKeyState(VK_ESCAPE) & 0x8000)) {
                kb.isListening = false;
                return;
            }

            if (!kb.skipFrame) {
                for (int keyCode = 1; keyCode < 256; ++keyCode) {
                    if (GetAsyncKeyState(keyCode) & 0x8000) {
                        kb.key = keyCode;
                        kb.isListening = false;
                        return;
                    }
                }
            }
            else {
                kb.skipFrame = false;
            }
        }
    }
}

Keybind* Keybinds::getKeybind(bool& var) {
    for (auto& keybind : keybinds) {
        if (&keybind.var == &var) {
            return &keybind;
        }
    }
    return nullptr;
}

Keybinds keybind;
