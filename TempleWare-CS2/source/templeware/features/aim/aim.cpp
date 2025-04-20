#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"
#include "../../../templeware/interfaces/CGameEntitySystem/CGameEntitySystem.h"
#include "../../../templeware/interfaces/interfaces.h"
#include "../../../templeware/hooks/hooks.h"
#include "../../../templeware/config/config.h"
#include "../../utils/math/vector/vector.h"

#include <chrono>
#include <Windows.h>
#include <cmath>
#include <algorithm>

// Literally the most autistic code ive ever written in my life
// Please dont ever make me do this again

// Prevent macro interference with min/max
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

template<typename T>
T clamp(T value, T min_val, T max_val) {
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

QAngle_t CalcAngles(const Vector_t& src, const Vector_t& dst) {
    QAngle_t angles = { 0, 0, 0 };
    Vector_t delta = dst - src;

    float hyp = delta.Length2D();
    if (hyp > 0.0f) {
        angles.x = -atan2f(delta.z, hyp) * (180.0f / 3.141592654f);
        angles.y = atan2f(delta.y, delta.x) * (180.0f / 3.141592654f);
    }

    return angles.Normalize();
}

float GetFov(const QAngle_t& view_angle, const QAngle_t& aim_angle) {
    QAngle_t delta = (aim_angle - view_angle).Normalize();
    return delta.Length2D();
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

    QAngle_t smoothed;
    smoothed.x = current.x + delta.x * smooth_factor;
    smoothed.y = current.y + delta.y * smooth_factor;
    smoothed.z = 0.0f;

    return smoothed.Normalize();
}

bool IsVisible(const Vector_t& start, const Vector_t& end, const C_CSPlayerPawn* target) {
    if (!target || !target->getHealth() || target->getHealth() <= 0) {
        return false;
    }

    C_CSPlayerPawn* local_player = H::oGetLocalPlayer(0);
    if (!local_player || local_player->getHealth() <= 0) {
        return false;
    }


    if (local_player->getTeam() == target->getTeam()) {
        return false;
    }


    if (!std::isfinite(start.x) || !std::isfinite(start.y) || !std::isfinite(start.z)) {
        return false;
    }


    const int bones[] = { 6, 5, 4 }; // Bone indices: 6 (head), 5 (neck), 4 (chest)
    const int bone_count = Config::aimbot_multipoint ? clamp(Config::aimbot_multipoint_count, 1, 3) : 1;


    for (int i = 0; i < bone_count; ++i) {
        int bone_index = bones[i];
        Vector_t bone_pos = GetBonePosition(target, bone_index);


        if (bone_pos.IsZero() || !std::isfinite(bone_pos.x) || !std::isfinite(bone_pos.y) || !std::isfinite(bone_pos.z)) {
            continue;
        }


        float distance = (bone_pos - start).Length();
        if (distance > Config::aimbot_max_distance) {
            continue;
        }


        QAngle_t view_angles = *reinterpret_cast<QAngle_t*>(modules.getModule("client") + 0x1A933C0);
        QAngle_t angle_to_bone = CalcAngles(start, bone_pos);
        float fov = GetFov(view_angles, angle_to_bone);
        if (fov > Config::aimbot_fov) {
            continue;
        }


        Vector_t delta = bone_pos - start;
        float height_diff = std::abs(delta.z);
        if (height_diff > 128.0f && distance < 256.0f) {
            continue;
        }


        if (distance > 0.0f) {
            Vector_t normalized_dir = delta.Normalize();

            float dot_product = normalized_dir.x * 0.0f + normalized_dir.y * 0.0f + normalized_dir.z * 1.0f;
            if (std::abs(dot_product) > 0.9f && height_diff > 64.0f) {
                continue;
            }
        }


        return true;
    }


    return false;
}

void Aimbot() {
    if (!Config::aimbot) return;

    static auto last_time = std::chrono::high_resolution_clock::now();
    static bool was_aimbot_enabled = false;
    auto current_time = std::chrono::high_resolution_clock::now();
    float delta_time = std::chrono::duration<float>(current_time - last_time).count();
    last_time = current_time;

    delta_time = clamp(delta_time, 0.001f, 0.1f);

    C_CSPlayerPawn* local_player = H::oGetLocalPlayer(0);
    if (!local_player || local_player->getHealth() <= 0) return;

    Vector_t local_eye_pos = GetEntityEyePos(local_player);
    QAngle_t* view_angles = reinterpret_cast<QAngle_t*>(modules.getModule("client") + 0x1A933C0);
    bool is_attacking = GetAsyncKeyState(VK_LBUTTON) & 0x8000;


    bool initial_activation = !was_aimbot_enabled && Config::aimbot;
    was_aimbot_enabled = Config::aimbot;

    float best_fov = Config::aimbot_fov;
    QAngle_t best_angle = *view_angles;
    bool found_target = false;

    // Define bones for multipoint aiming (head, neck, chest)
    const int bones[] = { 6, 5, 4 }; // Bone indices: 6 (head), 5 (neck), 4 (chest)
    const int bone_count = Config::aimbot_multipoint ? clamp(Config::aimbot_multipoint_count, 1, 3) : 1;

    int max_entities = I::GameEntity->Instance->GetHighestEntityIndex();
    for (int i = 1; i <= max_entities; ++i) {
        auto entity = I::GameEntity->Instance->Get(i);
        if (!entity || !entity->handle().valid()) continue;

        SchemaClassInfoData_t* class_info = nullptr;
        entity->dump_class_info(&class_info);
        if (!class_info || hash_32_fnv1a_const(class_info->szName) != hash_32_fnv1a_const("C_CSPlayerPawn")) {
            continue;
        }

        C_CSPlayerPawn* pawn = reinterpret_cast<C_CSPlayerPawn*>(entity);
        if (pawn->get_entity_by_handle() == local_player->get_entity_by_handle() ||
            pawn->getHealth() <= 0 ||
            pawn->getTeam() == local_player->getTeam()) {
            continue;
        }

        // Iterate through selected bones
        for (int j = 0; j < bone_count; ++j) {
            int bone_index = bones[j];
            Vector_t bone_pos = GetBonePosition(pawn, bone_index);
            if (bone_pos.IsZero()) continue;

            // Visibility check: Only apply if wall check is enabled
            if (Config::aimbot_wall_check && !IsVisible(local_eye_pos, bone_pos, pawn)) continue;

            QAngle_t angle = CalcAngles(local_eye_pos, bone_pos);
            if (!angle.IsValid()) continue;

            float fov = GetFov(*view_angles, angle);
            if (!std::isfinite(fov) || fov > best_fov) continue;

            // Recoil compensation
            if (Config::rcs && is_attacking) {
                QAngle_t aim_punch = *reinterpret_cast<QAngle_t*>((uintptr_t)local_player + SchemaFinder::Get(hash_32_fnv1a_const("C_CSPlayerPawn->m_aimPunchAngle")));
                angle -= aim_punch * Config::rcs_scale;
            }

            best_fov = fov;
            best_angle = angle;
            found_target = true;
        }
    }

    if (found_target) {

        best_angle = SmoothAngles(*view_angles, best_angle, Config::aimbot_smoothness, delta_time, initial_activation);

        best_angle.x = clamp(best_angle.x, -89.0f, 89.0f);
        best_angle.y = std::fmodf(best_angle.y + 180.0f, 360.0f) - 180.0f;
        best_angle.z = 0.0f;

        *view_angles = best_angle;
    }
}