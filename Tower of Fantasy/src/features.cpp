#include "features.hpp"
#include "utility/menu_features/all.hpp"
#include "utility/menu_settings.hpp"
#include "script.hpp"

namespace ellohim::features
{
	void run_per_tick()
	{
        movement::infinite_jump(g_settings->player.infinite_jump);
        movement::infinite_dodge(g_settings->player.infinite_dodge);

        misc::attack_multiplier(g_settings->player.attack_multiplier);
        attack_stats::pysical_attack(g_settings->player.pysical_attack);
        attack_stats::critical_damage(g_settings->player.critical_damage);
        attack_stats::critical_rate(g_settings->player.critical_rate);
        attack_stats::rapid_attack(g_settings->player.fast_attack);

        movement::no_clip(g_settings->player.no_clip);
        movement::set_player_movement_speed(g_settings->player.movement_speed);
        movement::set_player_crouch_speed(g_settings->player.crouch_speed);
        movement::set_player_swim_speed(g_settings->player.swim_speed);
        movement::set_player_gravity(g_settings->player.gravity);
	}

    void script_func()
    {
        while (true)
        {
            TRY_CLAUSE
            {
                run_per_tick();
            }
            EXCEPT_CLAUSE
            script::get_current()->yield();
        }
    }
}