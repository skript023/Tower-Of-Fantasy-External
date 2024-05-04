#include "player_stats.h"
#include "imgui.h"
#include "bot_function.hpp"
#include <memory/process.hpp>
#include <utility/menu_features/all.hpp>
#include "utility/menu_settings.hpp"


namespace ellohim
{
    void player_stat::render_player_stat()
    {
        if (ImGui::BeginTabItem("Player Stats"))
        {
            g_settings->player.player_level = misc::get_player_level();
            if (ImGui::SliderInt("Player Level", &g_settings->player.player_level, 0, 100))
                misc::set_player_level(g_settings->player.player_level);

            g_settings->player.attack_multiplier = misc::get_attack_multiplier();
            ImGui::SliderFloat("Attack Multiplier", &g_settings->player.attack_multiplier, 0.0f, 9999.f);

            g_settings->player.pysical_attack = attack_stats::get_pysical_attack();
            ImGui::SliderFloat("Pysical Attack", &g_settings->player.pysical_attack, 0.0f, 9999.f);

            g_settings->player.critical_damage = attack_stats::get_critical_damage();
            ImGui::SliderFloat("Critical Damage", &g_settings->player.critical_damage, 0.0f, 9999.f);

            g_settings->player.critical_rate = attack_stats::get_critical_rate();
            ImGui::SliderFloat("Critical Rate", &g_settings->player.critical_rate, 0.0f, 100.f);

            static const float default_move = movement::get_player_movement_speed();
            g_settings->player.movement_speed = movement::get_player_movement_speed();
            ImGui::SliderFloat("Movement Speed", &g_settings->player.movement_speed, default_move, 9999.f);

            static const float default_swim = movement::get_player_swim_speed();
            g_settings->player.swim_speed = movement::get_player_swim_speed();
            ImGui::SliderFloat("Swim Speed", &g_settings->player.swim_speed, default_swim, 9999.f);

            static const float default_crouch = movement::get_player_crouch_speed();
            g_settings->player.crouch_speed = movement::get_player_crouch_speed();
            ImGui::SliderFloat("Crouch Speed", &g_settings->player.crouch_speed, default_crouch, 9999.f);

            g_settings->player.gravity = movement::get_player_gravity();
            ImGui::SliderFloat("Gravity", &g_settings->player.gravity, 0.f, 9999.f);



            constexpr int max = INT32_MAX;
            auto Address = 0x700FDD0;
            ImGui::EndTabItem();
        }
    }
}