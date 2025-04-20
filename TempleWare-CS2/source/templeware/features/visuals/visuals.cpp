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
#include "spotted.h"
using namespace Esp;

LocalPlayerCached cached_local;
std::vector<PlayerCache> cached_players;

void Visuals::init() {
    viewMatrix.viewMatrix = (viewmatrix_t*)M::getAbsoluteAddress(M::patternScan("client", "48 8D 0D ? ? ? ? 48 C1 E0 06"), 3, 0);
}

void Esp::cache()
{
    if (!I::EngineClient->valid())
        return;

    /*  Old method READ ME
    * @ // @here: manually cache once all existing entitys
        // to avoid issues when injecting mid game and hkAddEnt not called by game on existing Entity's

       int highest_index = I::GameEntity->Instance->GetHighestEntityIndex();
        for (int i = 1; i <= highest_index; i++) {
            auto entity = I::GameEntity->Instance->Get(i);
            if (!entity)
                continue;

            uintptr_t entityPointer = reinterpret_cast<uintptr_t>(entity);

            SchemaClassInfoData_t* entityInfo = nullptr;
            GetSchemaClassInfo(entityPointer, &entityInfo);
            if (!entityInfo) continue;

            if (strcmp(entityInfo->szName, "C_CSPlayerPawn") == 0) {
                bool exists = std::any_of(Players::pawns.begin(), Players::pawns.end(),
                    [&](const C_CSPlayerPawn& pawn) { return pawn.getAddress() == entityPointer; });
                if (!exists) {
                    Players::pawns.emplace_back(entityPointer);
                    std::cout << "Added pawn " << Players::pawns.size() << "\n";
                }
                continue;
            }

            if (strcmp(entityInfo->szName, "CCSPlayerController") == 0) {
                bool exists = std::any_of(Players::controllers.begin(), Players::controllers.end(),
                    [&](const CCSPlayerController& controller) { return controller.getAddress() == entityPointer; });
                if (!exists) {
                    Players::controllers.emplace_back(entityPointer);
                }
                continue;
            }
    }*/

    cached_players.clear();

    int nMaxHighestEntity = I::GameEntity->Instance->GetHighestEntityIndex();

    for (int i = 1; i <= nMaxHighestEntity; i++)
    {
        auto Entity = I::GameEntity->Instance->Get(i);
        if (!Entity)
            continue;

        if (!Entity->handle().valid())
            continue;

        SchemaClassInfoData_t* _class = nullptr;
        Entity->dump_class_info(&_class);
        if (!_class)
            continue;

        const uint32_t hash = HASH(_class->szName);

        PlayerType_t type = none;

        if (hash == HASH("CCSPlayerController"))
        {

            type = none; int health = 0;
            Vector_t position; Vector_t viewOffset;
            const char* name = "none"; const char* weapon_name = "none";

            CCSPlayerController* Controller = reinterpret_cast<CCSPlayerController*>(Entity);
            if (!Controller)
                continue;

            if (!Controller->m_hPawn().valid())
                continue;

            //@handle caching local player 
            if (Controller->IsLocalPlayer()) {
                auto LocalPlayer = I::GameEntity->Instance->Get<C_CSPlayerPawn>(Controller->m_hPawn().index());
                if (!LocalPlayer) {
                    cached_local.reset();
                    continue;
                }

                cached_local.alive = LocalPlayer->m_iHealth() > 0;
                if (LocalPlayer->m_iHealth() > 0) {
                    cached_local.poisition = LocalPlayer->m_vOldOrigin();
                    cached_local.health = LocalPlayer->m_iHealth();
                    cached_local.handle = LocalPlayer->handle().index();
                    cached_local.team = LocalPlayer->m_iTeamNum();
                }
                else {
                    cached_local.reset();
                }
            }
            else // @handle only players
            {
                auto Player = I::GameEntity->Instance->Get<C_CSPlayerPawn>(Controller->m_hPawn().index());
                if (!Player)
                    continue;

                if (Player->m_iHealth() <= 0)
                    continue;

                health = Player->m_iHealth();
                name = Controller->m_sSanitizedPlayerName();
                position = Player->m_vOldOrigin(); viewOffset = Player->m_vecViewOffset();

                cached_players.emplace_back(Entity, Player, Player->handle(),
                    type, health, name,
                    weapon_name, position, viewOffset, Player->m_iTeamNum());
            }
        }
    }
}
void RunRadarHack(C_CSPlayerPawn* localPawn) {
    if (!localPawn || !Config::radar) return;

    const int localTeam = localPawn->m_iTeamNum();

    for (const auto& player : cached_players) {
        if (!player.handle.valid() ||
            player.health <= 0 ||
            player.team_num == localTeam) continue;

        if (C_CSPlayerPawn* pawn = I::GameEntity->Instance->Get<C_CSPlayerPawn>(player.handle.index())) {
            // Устанавливаем флаг spotted
            *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(pawn) + offsets::m_bSpotted) = true;

            // Обновляем маску видимости для нашей команды
            uint32_t* spottedMask = reinterpret_cast<uint32_t*>(
                reinterpret_cast<uintptr_t>(pawn) + offsets::m_bSpottedByMask
                );

            *spottedMask |= (1 << (localTeam % 32)); // Устанавливаем бит нашей команды
        }
    }
}

void Visuals::esp() {
    // Only proceed if at least one ESP component is enabled
    if (!Config::esp && !Config::showHealth && !Config::espFill && !Config::showNameTags && !Config::radar) {
        return; // Exit early if no component is enabled
    }

    //@better example of getting local pawn
    C_CSPlayerPawn* localPawn = H::oGetLocalPlayer(0);
    if (!localPawn) {
        return;
    }

    // Получаем команду локального игрока напрямую
    int localTeam = localPawn->m_iTeamNum();

    if (cached_players.empty())
        return;

    for (const auto& Player : cached_players)
    {
        if (!Player.handle.valid() || Player.health <= 0 || Player.handle.index() == INVALID_EHANDLE_INDEX)
            continue;

        // Проверяем команду напрямую, используя localTeam
        if (Config::teamCheck && (Player.team_num == localTeam))
            continue;

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        Vector_t feetPos = Player.position;
        Vector_t headPos = Player.position + Player.viewOffset;

        Vector_t feetScreen, headScreen;
        if (!viewMatrix.WorldToScreen(feetPos, feetScreen) ||
            !viewMatrix.WorldToScreen(headPos, headScreen))
            continue;

        float boxHeight = (feetScreen.y - headScreen.y) * 1.3f;
        float boxWidth = boxHeight / 2.0f;

        float centerX = (feetScreen.x + headScreen.x) / 2.0f;
        float boxX = centerX - (boxWidth / 2.0f);

        float boxY = headScreen.y - (boxHeight - (feetScreen.y - headScreen.y)) / 2.0f;

        ImVec4 espColorWithAlpha = Config::espColor;
        espColorWithAlpha.w = Config::espFillOpacity;
        ImU32 boxColor = ImGui::ColorConvertFloat4ToU32(Config::espColor);
        ImU32 fillColor = ImGui::ColorConvertFloat4ToU32(espColorWithAlpha);

        // ESP Fill
        if (Config::espFill) {
            drawList->AddRectFilled(
                ImVec2(boxX, boxY),
                ImVec2(boxX + boxWidth, boxY + boxHeight),
                fillColor
            );
        }

        // ESP Box - only render if Config::esp is enabled
        if (Config::esp) {
            drawList->AddRect(
                ImVec2(boxX, boxY),
                ImVec2(boxX + boxWidth, boxY + boxHeight),
                boxColor,
                0.0f,
                0,
                Config::espThickness
            );
        }


        // Health Bar
        if (Config::showHealth) {
            int health = Player.health;
            float healthHeight = boxHeight * (static_cast<float>(health) / 100.0f);
            float barWidth = 4.0f;
            float barX = boxX - (barWidth + 2);
            float barY = boxY + (boxHeight - healthHeight);

            // Фон полоски
            drawList->AddRectFilled(
                ImVec2(barX, boxY),
                ImVec2(barX + barWidth, boxY + boxHeight),
                IM_COL32(70, 70, 70, 255)
            );

            // Заполнение здоровья
            ImU32 healthColor = IM_COL32(
                static_cast<int>((100 - health) * 2.55f),
                static_cast<int>(health * 2.55f),
                0,
                255
            );
            drawList->AddRectFilled(
                ImVec2(barX, barY),
                ImVec2(barX + barWidth, barY + healthHeight),
                healthColor
            );

            // Текст со значением HP
            std::string displayText = "[" + std::to_string(health) + "HP]";
            float fontScale = 0.75f;
            const ImFont* font = ImGui::GetFont();
            float fontSize = font->FontSize * fontScale;
            ImVec2 textSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, displayText.c_str());

            // Сдвигаем текст слева от барa и выравниваем по верхнему краю заливки
            float textX = barX - textSize.x - 4.0f;
            float textY = barY;  // именно на уровне верхней границы зелёной полосы

            // Рисуем тень
            drawList->AddText(
                font, fontSize,
                ImVec2(textX + 1, textY + 1),
                IM_COL32(0, 0, 0, 255),
                displayText.c_str()
            );
            // И сам текст
            drawList->AddText(
                font, fontSize,
                ImVec2(textX, textY),
                IM_COL32(255, 255, 255, 255),
                displayText.c_str()
            );
        }
        if (Config::showNameTags) {
            std::string playerName = Player.name;
            ImVec2 nameSize = ImGui::CalcTextSize(playerName.c_str());

            float nameX = boxX + (boxWidth - nameSize.x) / 2;
            float nameY = boxY - nameSize.y - 2;

            //@FIXME: shit method to do outline
            drawList->AddText(
                ImVec2(nameX + 1, nameY + 1),
                IM_COL32(0, 0, 0, 255),
                playerName.c_str()
            );

            drawList->AddText(
                ImVec2(nameX, nameY),
                IM_COL32(255, 255, 255, 255),
                playerName.c_str()
            );
        }
    }
}