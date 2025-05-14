#pragma once
#include "../../../external/imgui/imgui.h"

// CBA to make proper atm, it's 03:42 right now.
// For now just stores config values don't mind it too much
//
// (FYI THIS IS A HORRID SOLUTION BUT FUNCTIONS) 

namespace Config {
	extern bool esp;
	extern bool showHealth;
	extern bool teamCheck;
	extern bool espFill;
	extern float espThickness;
	extern float espFillOpacity;
	extern ImVec4 espColor;
	extern bool showNameTags;

	extern bool Night;
	extern ImVec4 NightColor;

	extern bool enemyChamsInvisible;
	extern bool enemyChams;
	extern bool teamChams;
	extern bool teamChamsInvisible;
	extern int chamsMaterial;

	extern ImVec4 colVisualChams;
	extern ImVec4 colVisualChamsIgnoreZ;
	extern ImVec4 teamcolVisualChamsIgnoreZ;
	extern ImVec4 teamcolVisualChams;

	extern bool armChams;
	extern bool viewmodelChams;
	extern ImVec4 colViewmodelChams;
	extern ImVec4 colArmChams;

	extern bool fovEnabled;
	extern float fov;

	extern bool antiflash;

	extern bool Night;

	extern bool aimbot;
	extern float aimbot_fov;
	extern bool rcs;
	extern bool fov_circle;
	extern ImVec4 fovCircleColor;
	extern float aimbot_smoothness;
	extern float aimbot_randomness;
	extern bool show_keybinds;
	extern bool radar;
	static float rcs_scale = 2.0f;
	extern bool aimbot_multipoint; // New: Enable multipoint aiming
	extern int aimbot_multipoint_count; // New: Number of bones to target (1-3)
	extern float aimbot_max_distance;
	extern bool aimbot_wall_check;
	// New members
	extern bool show_watermark;
	extern ImVec2 watermarkPos;
	extern ImVec2 keybindPos;
	extern bool BunnyHop;

}