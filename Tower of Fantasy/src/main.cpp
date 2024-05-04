#include "renderer.hpp"
#include "Functions.h"
#include "memory/process.hpp"
#include "pointers.hpp"
#include "common.hpp"
#include "file_manager.hpp"
#include "thread_pool.hpp"
#include "fiber_pool.hpp"
#include "script_mgr.hpp"
#include "features.hpp"
#include "utility/menu_settings.hpp"
#include "utility/utility.hpp"

inline uint32_t find_game_id() 
{
	using namespace ellohim;

	auto win = ellohim::functions::GetProcessId(ellohim::m_target_process);
	if (!win) 
	{
		LOG(INFO) << "Cannot find game window";
	} 

	return win;
}

int main()
{
	using namespace ellohim;

	overlay_window::m_name = "Ellohim Tower of Fantasy";

	std::filesystem::path base_dir = std::getenv("appdata");
	base_dir /= ENVIRONMENT;

	try
	{
		auto file_manager_instance = std::make_unique<file_manager>(base_dir);

		auto logger_instance = std::make_unique<logger>(
			overlay_window::m_name,
			file_manager_instance->get_project_file("./Tower of Fantasy.log"));

		LOG(RAW_GREEN_TO_CONSOLE) << R"kek(
__          __    _                   _  __           _        _ 
\ \        / /   | |                 | |/ /          | |      | |
 \ \  /\  / /   _| |__   __ _ _ __   | ' / ___  _ __ | |_ ___ | |
  \ \/  \/ / | | | '_ \ / _` | '_ \  |  < / _ \| '_ \| __/ _ \| |
   \  /\  /| |_| | | | | (_| | | | | | . \ (_) | | | | || (_) | |
    \/  \/  \__,_|_| |_|\__,_|_| |_| |_|\_\___/|_| |_|\__\___/|_|
 )kek";

		LOG(INFO) << "Waiting for " << m_target_process;

		while (!FindWindow("UnrealWindow", "Tower of Fantasy  "))
			Sleep(500);

		auto process_instance = std::make_unique<process>(find_game_id());
		LOG(INFO) << "Process initalized.";

		auto pointers_instance = std::make_unique<pointers>();
		LOG(INFO) << "Pointers initialized.";

		LOG(INFO) << "Waiting for game loaded";

		while (!utility::wait_game())
			Sleep(500);

		LOG(INFO) << "Game is fully loaded";

		auto fiber_pool_instance = std::make_unique<fiber_pool>(10);
		LOG(INFO) << "Fiber pool initialized.";

		auto thread_pool_instance = std::make_unique<thread_pool>();
		LOG(INFO) << "Thread pool initialized.";

		auto renderer_instance = std::make_unique<renderer>();
		LOG(INFO) << "Renderer Initialized.";

		g_script_mgr.add_script(std::make_unique<script>(&features::script_func));
		LOG(INFO) << "Scripts registered.";

		auto settings_instance = std::make_unique<settings>();
		g_settings->load();
		LOG(INFO) << "Settings initialized.";

		while (g_running && g_process->is_running())
		{
			g_script_mgr.tick();
			g_renderer->render_on_tick();

			Sleep(0);
		}

		renderer_instance.reset();
		LOG(INFO) << "Renderer Uninitialized";

		Sleep(1000);

		g_script_mgr.remove_all_scripts();
		LOG(INFO) << "Scripts unregistered.";

		fiber_pool_instance.reset();
		LOG(INFO) << "Fiber pool uninitialized.";

		thread_pool_instance->destroy();
		LOG(INFO) << "Destroyed thread pool.";

		thread_pool_instance.reset();
		LOG(INFO) << "Thread pool uninitialized.";

		pointers_instance.reset();
		LOG(INFO) << "Pointers Uninitialized.";

		process_instance.reset();
		LOG(INFO) << "Process Uninitalized.";

		logger_instance.reset();
		file_manager_instance.reset();

		g_settings->attempt_save();
		settings_instance.reset();
		LOG(INFO) << "Settings saved and uninitialized.";
	}
	catch (std::exception& const  ex)
	{
		LOG(WARNING) << ex.what();
		MessageBoxA(nullptr, ex.what(), nullptr, MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}