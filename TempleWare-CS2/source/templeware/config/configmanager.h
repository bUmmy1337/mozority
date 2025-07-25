#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdlib>

#include "config.h"
#include "../../../external/json/json.hpp"

namespace internal_config
{
    class ConfigManager
    {
    private:

        static std::filesystem::path GetConfigFolder()
        {
            char* userProfile = nullptr;
            size_t len = 0;
            errno_t err = _dupenv_s(&userProfile, &len, "USERPROFILE");

            std::filesystem::path folder;
            if (err != 0 || userProfile == nullptr || len == 0)
            {
                folder = ".mozority";
            }
            else
            {
                folder = userProfile;
                free(userProfile);
                folder /= ".mozority";
            }
            folder /= "internal";

            std::error_code ec;
            std::filesystem::create_directories(folder, ec);

            return folder;
        }

        static std::filesystem::path GetConfigPath(const std::string& configName)
        {
            auto folder = GetConfigFolder();
            return folder / (configName + ".json");
        }

    public:

        static std::vector<std::string> ListConfigs()
        {
            std::vector<std::string> list;
            auto folder = GetConfigFolder();
            if (!std::filesystem::exists(folder))
                return list;

            for (const auto& entry : std::filesystem::directory_iterator(folder))
            {
                if (entry.is_regular_file())
                {
                    auto path = entry.path();
                    if (path.extension() == ".json")
                    {
                        list.push_back(path.stem().string());
                    }
                }
            }
            return list;
        }

        static void Save(const std::string& configName)
        {
            nlohmann::json j;
            j["esp"] = Config::esp;
            j["showHealth"] = Config::showHealth;
            j["teamCheck"] = Config::teamCheck;
            j["espFill"] = Config::espFill;
            j["espThickness"] = Config::espThickness;
            j["espFillOpacity"] = Config::espFillOpacity;

            j["fovEnabled"] = Config::fovEnabled;
            j["fov"] = Config::fov;

            j["espColor"] = {
                Config::espColor.x,
                Config::espColor.y,
                Config::espColor.z,
                Config::espColor.w
            };

            j["Night"] = Config::Night;
            j["NightColor"] = {
                Config::NightColor.x,
                Config::NightColor.y,
                Config::NightColor.z,
                Config::NightColor.w
            };

            j["armChams"] = Config::armChams;
            j["viewmodelChams"] = Config::viewmodelChams;

            j["armChams_color"] = {
                Config::colArmChams.x,
                Config::colArmChams.y,
                Config::colArmChams.z,
                Config::colArmChams.w
            };

            j["viewmodelChams_color"] = {
                Config::colViewmodelChams.x,
                Config::colViewmodelChams.y,
                Config::colViewmodelChams.z,
                Config::colViewmodelChams.w
            };

            j["aimbot"] = Config::aimbot;
            j["aimbot_fov"] = Config::aimbot_fov;
            j["antiflash"] = Config::antiflash;
            j["rcs"] = Config::rcs;
            j["fov_circle"] = Config::fov_circle;

            j["enemyChamsInvisible"] = Config::enemyChamsInvisible;
            j["enemyChams"] = Config::enemyChams;
            j["teamChams"] = Config::teamChams;
            j["teamChamsInvisible"] = Config::teamChamsInvisible;
            j["chamsMaterial"] = Config::chamsMaterial;
            j["colVisualChams"] = {
                Config::colVisualChams.x,
                Config::colVisualChams.y,
                Config::colVisualChams.z,
                Config::colVisualChams.w
            };
            j["colVisualChamsIgnoreZ"] = {
                Config::colVisualChamsIgnoreZ.x,
                Config::colVisualChamsIgnoreZ.y,
                Config::colVisualChamsIgnoreZ.z,
                Config::colVisualChamsIgnoreZ.w
            };
            j["teamcolVisualChamsIgnoreZ"] = {
                Config::teamcolVisualChamsIgnoreZ.x,
                Config::teamcolVisualChamsIgnoreZ.y,
                Config::teamcolVisualChamsIgnoreZ.z,
                Config::teamcolVisualChamsIgnoreZ.w
            };
            j["teamcolVisualChams"] = {
                Config::teamcolVisualChams.x,
                Config::teamcolVisualChams.y,
                Config::teamcolVisualChams.z,
                Config::teamcolVisualChams.w
            };
            j["fovCircleColor"] = {
                Config::fovCircleColor.x,
                Config::fovCircleColor.y,
                Config::fovCircleColor.z,
                Config::fovCircleColor.w
            };

            // Added missing variables
            j["showNameTags"] = Config::showNameTags;
            j["show_keybinds"] = Config::show_keybinds;
            j["aimbot_randomness"] = Config::aimbot_randomness;
            j["aimbot_smoothness"] = Config::aimbot_smoothness;
            j["radar"] = Config::radar;
            j["aimbot_multipoint"] = Config::aimbot_multipoint;
            j["aimbot_multipoint_count"] = Config::aimbot_multipoint_count;
            j["aimbot_max_distance"] = Config::aimbot_max_distance;
            j["aimbot_wall_check"] = Config::aimbot_wall_check;
            j["show_watermark"] = Config::show_watermark;
            j["watermarkPos"] = { Config::watermarkPos.x, Config::watermarkPos.y };
            j["keybindPos"] = { Config::keybindPos.x, Config::keybindPos.y };
            j["BunnyHop"] = Config::BunnyHop;

            auto filePath = GetConfigPath(configName);
            std::ofstream ofs(filePath);
            if (ofs.is_open())
            {
                ofs << j.dump(4);
                ofs.close();
            }
        }

        static void Load(const std::string& configName)
        {
            auto filePath = GetConfigPath(configName);
            if (!std::filesystem::exists(filePath))
                return;

            std::ifstream ifs(filePath);
            if (!ifs.is_open())
                return;

            nlohmann::json j;
            ifs >> j;

            Config::esp = j.value("esp", false);
            Config::showHealth = j.value("showHealth", false);
            Config::teamCheck = j.value("teamCheck", false);
            Config::espFill = j.value("espFill", false);
            Config::espThickness = j.value("espThickness", 1.0f);
            Config::espFillOpacity = j.value("espFillOpacity", 0.5f);

            Config::fovEnabled = j.value("fovEnabled", false);
            Config::fov = j.value("fov", 90.0f);

            if (j.contains("espColor") && j["espColor"].is_array() && j["espColor"].size() == 4)
            {
                auto arr = j["espColor"];
                Config::espColor.x = arr[0].get<float>();
                Config::espColor.y = arr[1].get<float>();
                Config::espColor.z = arr[2].get<float>();
                Config::espColor.w = arr[3].get<float>();
            }

            Config::Night = j.value("Night", false);
            if (j.contains("NightColor") && j["NightColor"].is_array() && j["NightColor"].size() == 4)
            {
                auto arr = j["NightColor"];
                Config::NightColor.x = arr[0].get<float>();
                Config::NightColor.y = arr[1].get<float>();
                Config::NightColor.z = arr[2].get<float>();
                Config::NightColor.w = arr[3].get<float>();
            }

            Config::enemyChamsInvisible = j.value("enemyChamsInvisible", false);
            Config::enemyChams = j.value("enemyChams", false);
            Config::teamChams = j.value("teamChams", false);
            Config::teamChamsInvisible = j.value("teamChamsInvisible", false);
            Config::chamsMaterial = j.value("chamsMaterial", 0);

            Config::fov_circle = j.value("fov_circle", false);
            Config::aimbot = j.value("aimbot", false);
            Config::rcs = j.value("rcs", false);
            Config::aimbot_fov = j.value("aimbot_fov", 0.f);

            Config::antiflash = j.value("antiflash", false);

            Config::armChams = j.value("armChams", false);
            Config::viewmodelChams = j.value("viewmodelChams", false);

            if (j.contains("colArmChams") && j["colArmChams"].is_array() && j["colArmChams"].size() == 4)
            {
                auto arr = j["colArmChams"];
                Config::colArmChams.x = arr[0].get<float>();
                Config::colArmChams.y = arr[1].get<float>();
                Config::colArmChams.z = arr[2].get<float>();
                Config::colArmChams.w = arr[3].get<float>();
            }

            if (j.contains("colViewmodelChams") && j["colViewmodelChams"].is_array() && j["colViewmodelChams"].size() == 4)
            {
                auto arr = j["colViewmodelChams"];
                Config::colViewmodelChams.x = arr[0].get<float>();
                Config::colViewmodelChams.y = arr[1].get<float>();
                Config::colViewmodelChams.z = arr[2].get<float>();
                Config::colViewmodelChams.w = arr[3].get<float>();
            }

            if (j.contains("colVisualChams") && j["colVisualChams"].is_array() && j["colVisualChams"].size() == 4)
            {
                auto arr = j["colVisualChams"];
                Config::colVisualChams.x = arr[0].get<float>();
                Config::colVisualChams.y = arr[1].get<float>();
                Config::colVisualChams.z = arr[2].get<float>();
                Config::colVisualChams.w = arr[3].get<float>();
            }

            if (j.contains("colVisualChamsIgnoreZ") && j["colVisualChamsIgnoreZ"].is_array() && j["colVisualChamsIgnoreZ"].size() == 4)
            {
                auto arr = j["colVisualChamsIgnoreZ"];
                Config::colVisualChamsIgnoreZ.x = arr[0].get<float>();
                Config::colVisualChamsIgnoreZ.y = arr[1].get<float>();
                Config::colVisualChamsIgnoreZ.z = arr[2].get<float>();
                Config::colVisualChamsIgnoreZ.w = arr[3].get<float>();
            }

            if (j.contains("teamcolVisualChamsIgnoreZ") && j["teamcolVisualChamsIgnoreZ"].is_array() && j["teamcolVisualChamsIgnoreZ"].size() == 4)
            {
                auto arr = j["teamcolVisualChamsIgnoreZ"];
                Config::teamcolVisualChamsIgnoreZ.x = arr[0].get<float>();
                Config::teamcolVisualChamsIgnoreZ.y = arr[1].get<float>();
                Config::teamcolVisualChamsIgnoreZ.z = arr[2].get<float>();
                Config::teamcolVisualChamsIgnoreZ.w = arr[3].get<float>();
            }

            if (j.contains("teamcolVisualChams") && j["teamcolVisualChams"].is_array() && j["teamcolVisualChams"].size() == 4)
            {
                auto arr = j["teamcolVisualChams"];
                Config::teamcolVisualChams.x = arr[0].get<float>();
                Config::teamcolVisualChams.y = arr[1].get<float>();
                Config::teamcolVisualChams.z = arr[2].get<float>();
                Config::teamcolVisualChams.w = arr[3].get<float>();
            }

            if (j.contains("fovCircleColor") && j["fovCircleColor"].is_array() && j["fovCircleColor"].size() == 4) {
                auto arr = j["fovCircleColor"];
                Config::fovCircleColor.x = arr[0].get<float>();
                Config::fovCircleColor.y = arr[1].get<float>();
                Config::fovCircleColor.z = arr[2].get<float>();
                Config::fovCircleColor.w = arr[3].get<float>();
            }

            // Added missing variables
            Config::showNameTags = j.value("showNameTags", false);
            Config::show_keybinds = j.value("show_keybinds", true);
            Config::aimbot_randomness = j.value("aimbot_randomness", 0.5f);
            Config::aimbot_smoothness = j.value("aimbot_smoothness", 0.5f);
            Config::radar = j.value("radar", false);
            Config::aimbot_multipoint = j.value("aimbot_multipoint", false);
            Config::aimbot_multipoint_count = j.value("aimbot_multipoint_count", 1);
            Config::aimbot_max_distance = j.value("aimbot_max_distance", 3000.0f);
            Config::aimbot_wall_check = j.value("aimbot_wall_check", true);
            Config::show_watermark = j.value("show_watermark", true);
            if (j.contains("watermarkPos") && j["watermarkPos"].is_array() && j["watermarkPos"].size() == 2)
            {
                auto arr = j["watermarkPos"];
                Config::watermarkPos.x = arr[0].get<float>();
                Config::watermarkPos.y = arr[1].get<float>();
            }
            else
            {
                Config::watermarkPos = ImVec2(10.0f, 10.0f);
            }
            if (j.contains("keybindPos") && j["keybindPos"].is_array() && j["keybindPos"].size() == 2)
            {
                auto arr = j["keybindPos"];
                Config::keybindPos.x = arr[0].get<float>();
                Config::keybindPos.y = arr[1].get<float>();
            }
            else
            {
                Config::keybindPos = ImVec2(10.0f, 300.0f);
            }
            Config::BunnyHop = j.value("BunnyHop", false);

            ifs.close();
        }

        static void Remove(const std::string& configName)
        {
            auto filePath = GetConfigPath(configName);
            if (std::filesystem::exists(filePath))
            {
                std::error_code ec;
                std::filesystem::remove(filePath, ec);
            }
        }
    };
}