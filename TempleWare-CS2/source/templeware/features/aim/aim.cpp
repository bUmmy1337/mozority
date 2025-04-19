#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../../templeware/interfaces/CGameEntitySystem/CGameEntitySystem.h"
#include "../../../templeware/interfaces/interfaces.h"
#include "../../../templeware/hooks/hooks.h"
#include "../../../templeware/config/config.h"

#include <chrono>
#include <Windows.h>
#include <cmath>
#include <algorithm>

// Literally the most autistic code ive ever written in my life
// Please dont ever make me do this again
// благодарю автора за этот пиздатый код в 10 строчек который я пофиксил костылями ебучими тварь я тебя ненавижу спасибо за аим

template<typename T>
T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Vector_t GetEntityEyePos(const C_CSPlayerPawn* Entity) {
    if (!Entity)
        return {};

    uintptr_t game_scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)Entity + SchemaFinder::Get(hash_32_fnv1a_const("C_BaseEntity->m_pGameSceneNode")));

    auto Origin = *reinterpret_cast<Vector_t*>(game_scene_node + SchemaFinder::Get(hash_32_fnv1a_const("CGameSceneNode->m_vecAbsOrigin")));
    auto ViewOffset = *reinterpret_cast<Vector_t*>((uintptr_t)Entity + SchemaFinder::Get(hash_32_fnv1a_const("C_BaseModelEntity->m_vecViewOffset")));

    Vector_t Result = Origin + ViewOffset;
    if (!std::isfinite(Result.x) || !std::isfinite(Result.y) || !std::isfinite(Result.z))
        return {};

    return Result;
}

inline QAngle_t CalcAngles(Vector_t viewPos, Vector_t aimPos)
{
    QAngle_t angle = { 0, 0, 0 };

    Vector_t delta = aimPos - viewPos;

    angle.x = -asin(delta.z / delta.Length()) * (180.0f / 3.141592654f);
    angle.y = atan2(delta.y, delta.x) * (180.0f / 3.141592654f);

    return angle;
}

inline float GetFov(const QAngle_t& viewAngle, const QAngle_t& aimAngle)
{
    QAngle_t delta = (aimAngle - viewAngle).Normalize();

    return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

QAngle_t SmoothAngles(const QAngle_t& current, const QAngle_t& target, float smoothness, float delta_time)
{
    if (smoothness <= 0.0f) {
        return target;
    }

    float time_constant = smoothness * 0.1f; // Чем больше smoothness, тем медленнее движение
    float smooth_factor = 1.0f - std::exp(-delta_time / time_constant);

    QAngle_t smoothed;
    smoothed.x = current.x + (target.x - current.x) * smooth_factor;
    smoothed.y = current.y + (target.y - current.y) * smooth_factor;
    smoothed.z = 0.0f;

    return smoothed.Normalize();
}

void Aimbot() {
    if (!Config::aimbot)
        return;

    static auto last_time = std::chrono::high_resolution_clock::now();
    auto current_time = std::chrono::high_resolution_clock::now();
    float delta_time = std::chrono::duration<float>(current_time - last_time).count();
    last_time = current_time;

    int nMaxHighestEntity = I::GameEntity->Instance->GetHighestEntityIndex();

    C_CSPlayerPawn* lp = H::oGetLocalPlayer(0);
    Vector_t lep = GetEntityEyePos(lp);

    QAngle_t* viewangles = (QAngle_t*)(modules.getModule("client") + 0x1A933C0);

    // Проверка, зажата ли левая кнопка мыши (MOUSE1)
    bool is_attacking = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

    for (int i = 1; i <= nMaxHighestEntity; i++) {
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

        if (hash == HASH("C_CSPlayerPawn")) {
            C_CSPlayerPawn* pawn = (C_CSPlayerPawn*)Entity;

            if (pawn->get_entity_by_handle() == lp->get_entity_by_handle())
                continue;

            if (pawn->getHealth() < 1)
                continue;

            if (pawn->getTeam() == lp->getTeam())
                continue;

            Vector_t eye_pos = GetEntityEyePos(pawn);
            eye_pos.x -= 5.0f; // Сдвиг в центр головы от затылка в ЕБАЛЬНИК
            eye_pos.z += 1.25f; // Сдвиг в центр головы поднятие к ЕБАЛЬНИКУ

            float randomness = Config::aimbot_randomness;
            Vector_t aim_pos = eye_pos + Vector_t{
                (rand() % 100 - 50) / 50.0f * randomness,
                (rand() % 100 - 50) / 50.0f * randomness,
                (rand() % 100 - 50) / 50.0f * randomness
            };

            QAngle_t angle = CalcAngles(lep, aim_pos);

            QAngle_t angle_to_target = CalcAngles(lep, eye_pos);
            const float fov = GetFov(*viewangles, angle_to_target);
            if (!std::isfinite(fov) || fov > Config::aimbot_fov)
                continue;

            if (Config::rcs && is_attacking) {
                QAngle_t ang_punch_angle = *(QAngle_t*)((uintptr_t)lp + SchemaFinder::Get(hash_32_fnv1a_const("C_CSPlayerPawn->m_aimPunchAngle")));
                angle -= ang_punch_angle * 2.f;
            }

            angle.z = 0.f;
            angle = angle.Normalize();

            *viewangles = SmoothAngles(*viewangles, angle, Config::aimbot_smoothness, delta_time);

            // Нормализация углов
            viewangles->x = std::clamp(viewangles->x, -89.f, 89.f); // Pitch
            viewangles->y = std::fmod(viewangles->y + 180.f, 360.f) - 180.f; // Yaw

            break;
        }
    }
}