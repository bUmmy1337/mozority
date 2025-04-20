#pragma once
#include <vector>

enum class KeybindMode {
    Toggle,
    Hold
};

struct Keybind {
    bool& var;
    int   key;
    KeybindMode mode;
    bool  isListening;
    bool  skipFrame;

    Keybind(bool& v, int k = 0, KeybindMode m = KeybindMode::Toggle);
};

class Keybinds {
public:
    Keybinds();
    void pollInputs();
    void menuButton(bool& var, bool renderCombo = true);
    Keybind* getKeybind(bool& var);
private:
    std::vector<Keybind> keybinds;
};

extern Keybinds keybind;