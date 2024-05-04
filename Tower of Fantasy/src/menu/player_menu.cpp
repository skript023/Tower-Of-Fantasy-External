#include "player_menu.h"
#include "imgui.h"
#include "bot_function.hpp"
#include "utility/menu_features/all.hpp"
#include "utility/menu_settings.hpp"
#include "utility/config/persist_teleport.h"
#include "script.hpp"

namespace ellohim
{
    void player_menu::add_value()
    {
    }

    void player_menu::render_player_tab()
    {
        if (ImGui::BeginTabItem("Player Option"))
        {
            auto end = std::chrono::system_clock::now();
            std::time_t date = std::chrono::system_clock::to_time_t(end);

            ImGui::Text("%s", std::ctime(&date));

            ImGui::Text("Resolution %dx%d", *g_pointers->m_screen_x, *g_pointers->m_screen_y);

            ImGui::BeginGroup();
            if (ImGui::Checkbox("God mode", &g_settings->player.godmode))
                misc::godmode(g_settings->player.godmode);

            ImGui::Checkbox("Infinite Jump", &g_settings->player.infinite_jump);

            if (ImGui::Checkbox("No Cooldown", &g_settings->player.no_cooldown))
                attack_stats::remove_cooldown(g_settings->player.no_cooldown);

            ImGui::Checkbox("ESP", &g_settings->player.esp);

            ImGui::EndGroup();

            ImGui::SameLine(200);

            ImGui::BeginGroup();

            if (ImGui::Checkbox("No Fall Damaage", &g_settings->player.no_fall_damage))
                misc::remove_fall_damage(g_settings->player.no_fall_damage);

            if (ImGui::Checkbox("Freeze Player", &g_settings->player.freeze_entity))
                misc::freeze_entity(g_settings->player.freeze_entity);

            if (ImGui::Checkbox("Freeze Mobs", &g_settings->player.freeze_mobs))
                misc::freeze_mobs(g_settings->player.freeze_mobs);

            ImGui::EndGroup();

            ImGui::SameLine(400);

            ImGui::BeginGroup();

            ImGui::Checkbox("Infinite Dodge", &g_settings->player.infinite_dodge);

            ImGui::Checkbox("Rapid Attack", &g_settings->player.fast_attack);

            ImGui::Checkbox("No Clip", &g_settings->player.no_clip);

            ImGui::EndGroup();

            if (ImGui::CollapsingHeader("Teleport Option"))
            {
                auto teleport_locations = persist_teleport::list_locations();
                static std::string selected_location;
                static char teleport_name[50]{};

                ImGui::PushItemWidth(200);
                ImGui::InputText("Location Name", teleport_name, IM_ARRAYSIZE(teleport_name));
                ImGui::PopItemWidth();

                ImGui::SameLine();

                if (ImGui::Button("Save Location"))
                {
                    g_fiber_pool->queue_job([]
                    {
                        persist_teleport::save_location(movement::get_entity_coords(), teleport_name);
                        ZeroMemory(teleport_name, sizeof(teleport_name));
                    });
                }

                ImGui::PushItemWidth(250);
                ImGui::Text("Saved Locations");
                if (ImGui::ListBoxHeader("##empty", ImVec2(200, 200)))
                {
                    for (auto pair : teleport_locations)
                    {
                        if (ImGui::Selectable(pair.c_str(), selected_location == pair))
                            selected_location = pair;
                    }
                    ImGui::ListBoxFooter();
                }
                ImGui::PopItemWidth();

                ImGui::SameLine();
                ImGui::BeginGroup();

                if (ImGui::Button("Load Location"))
                {
                    g_fiber_pool->queue_job([]
                    {
                        if (!selected_location.empty())
                        {
                            persist_teleport::load_location(selected_location);
                        }
                    });
                }
                if (ImGui::Button("Delete Location"))
                {
                    if (!selected_location.empty())
                    {
                        persist_teleport::delete_location(selected_location);
                        selected_location.clear();
                    }
                }
                ImGui::EndGroup();
            }

            ImGui::EndTabItem();
        }
    }
}