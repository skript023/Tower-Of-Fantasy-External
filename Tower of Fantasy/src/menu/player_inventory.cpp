#include "bot_function.hpp"
#include "player_inventory.h"
#include "imgui.h"
#include "utility/utility.hpp"

namespace ellohim
{
	void player_inventory::render_player_inventory()
	{
        if (ImGui::BeginTabItem("Player Inventory"))
        {
            

            ImGui::EndTabItem();
        }
	}
}