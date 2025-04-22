#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../../templeware/interfaces/CGameEntitySystem/CGameEntitySystem.h"
#include "../../../templeware/interfaces/interfaces.h"
#include "../../../templeware/hooks/hooks.h"
#include "../../../templeware/config/config.h"
#include "../../utils/math/vector/vector.h"
#include "../../../templeware/interfaces/IEngineClient/IEngineTrace.h"

#include <chrono>
#include <Windows.h>
#include <cmath>
#include <algorithm>
#include <iostream>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

template<typename T>
static T clamp(T value, T min_val, T max_val) {
    return std::max(min_val, std::min(max_val, value));
}

Vector_t GetBonePosition(const C_CSPlayerPawn* entity, int boneIndex) {
    if (!entity) return {};

    uintptr_t game_scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)entity + SchemaFinder::Get(hash_32_fnv1a_const("C_BaseEntity->m_pGameSceneNode")));
    if (!game_scene_node) return {};

    uintptr_t bone_array = *reinterpret_cast<uintptr_t*>(game_scene_node + SchemaFinder::Get(hash_32_fnv1a_const("CSkeletonInstance->m_modelState")) + 0x80);
    if (!bone_array) return {};

    Vector_t bone_pos = *reinterpret_cast<Vector_t*>(bone_array + boneIndex * 32);
    if (!std::isfinite(bone_pos.x) || !std::isfinite(bone_pos.y) || !std::isfinite(bone_pos.z)) {
        return {};
    }

    return bone_pos;
}

Vector_t GetEntityEyePos(const C_CSPlayerPawn* entity) {
    if (!entity) return {};

    uintptr_t game_scene_node = *reinterpret_cast<uintptr_t*>((uintptr_t)entity + SchemaFinder::Get(hash_32_fnv1a_const("C_BaseEntity->m_pGameSceneNode")));
    Vector_t origin = *reinterpret_cast<Vector_t*>(game_scene_node + SchemaFinder::Get(hash_32_fnv1a_const("CGameSceneNode->m_vecAbsOrigin")));
    Vector_t view_offset = *reinterpret_cast<Vector_t*>((uintptr_t)entity + SchemaFinder::Get(hash_32_fnv1a_const("C_BaseModelEntity->m_vecViewOffset")));

    Vector_t result = origin + view_offset;
    if (!std::isfinite(result.x) || !std::isfinite(result.y) || !std::isfinite(result.z)) {
        return {};
    }

    return result;
}

static QAngle_t CalcAngles(const Vector_t& src, const Vector_t& dst) {
    Vector_t delta = dst - src;
    float hyp = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    QAngle_t ang{};
    if (hyp > 0.0f) {
        ang.x = -std::atan2f(delta.z, hyp) * (180.0f / 3.14159265f);
        ang.y = std::atan2f(delta.y, delta.x) * (180.0f / 3.14159265f);
    }
    return ang.Normalize();
}

// разница углов
static float GetFov(const QAngle_t& view, const QAngle_t& aim) {
    QAngle_t d = (aim - view).Normalize();
    return std::sqrt(d.x * d.x + d.y * d.y);
}

QAngle_t SmoothAngles(const QAngle_t& current, const QAngle_t& target, float smoothness, float delta_time, bool initial_activation) {
    if (smoothness <= 0.0f) return target;

    QAngle_t delta = (target - current).Normalize();

    float distance = delta.Length2D();
    float adjusted_smoothness = smoothness;
    if (initial_activation && distance > 30.0f) {
        adjusted_smoothness *= 2.0f;
    }

    float smooth_factor = 1.0f - expf(-delta_time / std::max(adjusted_smoothness, 0.01f));
    smooth_factor = clamp(smooth_factor * 15.0f, 0.0f, 1.0f);

    QAngle_t smoothed;  // Properly declare the smoothed variable
    smoothed.x = current.x + delta.x * smooth_factor;
    smoothed.y = current.y + delta.y * smooth_factor;
    smoothed.z = 0.0f;

    return smoothed.Normalize();
}


// Фильтр трассировки, пропускающий локального игрока
class CTraceFilterSkip {
public:
    CTraceFilterSkip(C_CSPlayerPawn* skip) : m_pSkip(skip) {}
    bool ShouldHitEntity(void* entity, int contentsMask) {
        return entity != m_pSkip;
    }
    unsigned int GetTraceType() const { return 0; }
private:
    C_CSPlayerPawn* m_pSkip;
};

// Упрощённая структура результата (можно расширить поля по CS2 SDK)
struct trace_t {
    bool hit;
    C_CSPlayerPawn* entity;
    Vector_t endpos;
    float fraction;
};

static bool IsVisible(const Vector_t& start, const Vector_t& end, C_CSPlayerPawn* target) {
    if (!target || target->getHealth() <= 0) return false;
    C_CSPlayerPawn* local = H::oGetLocalPlayer(0);
    if (!local || local->getHealth() <= 0) return false;
    if (local->getTeam() == target->getTeam())    return false;
    if (!std::isfinite(start.x) || !std::isfinite(start.y) || !std::isfinite(start.z)) return false;
    if (!std::isfinite(end.x) || !std::isfinite(end.y) || !std::isfinite(end.z))   return false;
    if (!I::EngineTrace) {
        std::cout << "[Aim] EngineTrace = nullptr\n";
        return false;
    }

    TraceFilter filter{};
    filter.skipEntity = local;

    TraceResult result{};
    const unsigned mask = 0x200400B;  // MASK_VISIBLE
    I::EngineTrace->TraceRay(start, end, mask, &filter, &result);

    return (result.hit && result.entity == target);
}
void Aimbot() {
    if (!Config::aimbot) return;
    if (!I::EngineClient || !I::EngineTrace) {
        std::cout << "[Aim] ERR: EngineClient/EngineTrace null\n";
        return;
    }

    // базовые оффсеты для viewAngles
    uintptr_t clientBase = modules.getModule("client");
    constexpr ptrdiff_t OFF_VIEWANGLES = 0x1A93300;
    QAngle_t* viewAnglesPtr = reinterpret_cast<QAngle_t*>(clientBase + OFF_VIEWANGLES);

    C_CSPlayerPawn* local = H::oGetLocalPlayer(0);
    if (!local || local->getHealth() <= 0) {
        std::cout << "[Aim] ERR: local invalid/dead\n";
        return;
    }

    // читаем текущие углы
    QAngle_t view = *viewAnglesPtr;
    std::cout << "[Aim] initial view=(" << view.x << "," << view.y << ")\n";
    std::cout << "[Aim] config.fov=" << Config::aimbot_fov << "\n";

    // позиция глаза
    Vector_t eye = GetEntityEyePos(local);

    // задаём очень большой FOV для теста (180°)
    //float bestFov = 180.0f;
    float bestFov = Config::aimbot_fov; // вернуться к вашему параметру
    QAngle_t bestAng = view;
    bool found = false;

    int maxEnt = I::GameEntity->Instance->GetHighestEntityIndex();
    std::cout << "[Aim] maxEnt=" << maxEnt << "\n";

    for (int i = 1; i <= maxEnt; ++i) {
        auto ent = I::GameEntity->Instance->Get(i);
        if (!ent) continue;

        SchemaClassInfoData_t* ci = nullptr;
        ent->dump_class_info(&ci);
        if (!ci) continue;
        if (hash_32_fnv1a_const(ci->szName) != hash_32_fnv1a_const("C_CSPlayerPawn"))
            continue;

        C_CSPlayerPawn* pawn = reinterpret_cast<C_CSPlayerPawn*>(ent);
        if (pawn == local || pawn->getHealth() <= 0 || pawn->getTeam() == local->getTeam())
            continue;

        Vector_t targetPos = GetBonePosition(pawn, 6);
        if (targetPos.IsZero()) continue;

        bool vis = true;
        if (Config::aimbot_wall_check) {
            vis = IsVisible(eye, targetPos, pawn);
        }
        std::cout << "[Aim] Ent#" << i << " vis=" << vis << "\n";
        if (!vis) continue;

        QAngle_t ang = CalcAngles(eye, targetPos);
        float fov = GetFov(view, ang);
        std::cout << "[Aim] cand fov=" << fov << " ang=(" << ang.x << "," << ang.y << ")\n";

        if (fov < bestFov) {
            bestFov = fov;
            bestAng = ang;
            found = true;
            std::cout << "[Aim] new best fov=" << bestFov << "\n";
        }
    }

    if (found) {
        // плавность
        bestAng = SmoothAngles(view, bestAng, Config::aimbot_smoothness, 1 / 60.0f, true);
        bestAng.x = clamp(bestAng.x, -89.0f, 89.0f);
        bestAng.y = std::remainder(bestAng.y, 360.0f);
        bestAng.z = 0.0f;

        std::cout << "[Aim] setting angles=(" << bestAng.x << "," << bestAng.y << ")\n";
        *viewAnglesPtr = bestAng;

        // проверяем, записалось ли:
        QAngle_t verify = *viewAnglesPtr;
        std::cout << "[Aim] verify view=(" << verify.x << "," << verify.y << ")\n";
    }
}