#include "setting.h"
#include "imgui.h"
#include "utility/menu_features/all.hpp"


namespace ellohim
{
    void setting::render_setting_menu()
    {
        if (ImGui::BeginTabItem("Setting"))
        {
            if (ImGui::Button("Check Weapon Type"))
            {
                LOG(INFO) << "Weapon type : " << attack_stats::get_weapon_type();
            }

            ImGui::SameLine();

            if (ImGui::Button("Test"))
            {
                auto actor_array = memory::readPointer(g_pointers->m_world, { 0x30, 0x98 });
                int actor_count = memory::read<int>(g_pointers->m_world, { 0x30, 0xA0 });

                for (int i = 1; i < actor_count; ++i)
                {
                    uintptr_t actor = g_process->read<uintptr_t>(actor_array) + i * 0x8;
                    if (actor == NULL)
                    {
                        continue;
                    }

                    int id = memory::read<int>(actor, { 0x18 });
                    std::string namedump = utility::get_name_from_fname(id);
                    uint64_t root_component = memory::readPointer(actor, { 0x138, 0x0 });
                    if (root_component == NULL) continue;

                    Vector3 relativelocation = g_process->read<Vector3>(root_component + 0x124);

                    Vector3 cizpic = utility::world_to_screen(relativelocation);
                    LOG(INFO) << "location : " << " x " << cizpic.x << " y " << cizpic.y << " z " << cizpic.z;
                }
            }

            if (ImGui::Button("Quit"))
            {
                g_running = false;
            }

            ImGui::EndTabItem();
        }
    }
}