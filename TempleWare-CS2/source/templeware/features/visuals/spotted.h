#pragma once
#include "visuals.h"
#include <algorithm>
#include <iostream>
#include "../../hooks/hooks.h"
#include "../../players/players.h"
#include "../../utils/memory/patternscan/patternscan.h"
#include "../../utils/memory/gaa/gaa.h"
#include "../../../../external/imgui/imgui.h"
#include "../../interfaces/interfaces.h"
#include "../../config/config.h"
#include "../../menu/menu.h"
namespace offsets {
    constexpr uint32_t m_bSpotted = 0x8;      // Флаг видимости на радаре
    constexpr uint32_t m_bSpottedByMask = 0x8; // Битовая маска команд, видящих игрока
}