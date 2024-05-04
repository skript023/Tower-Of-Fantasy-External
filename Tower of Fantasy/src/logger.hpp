#pragma once
#include "common.hpp"
#include "file_manager.hpp"
#include "Functions.h"
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

namespace ellohim
{
	template <typename TP>
	std::time_t to_time_t(TP tp)
	{
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
			+ system_clock::now());
		return system_clock::to_time_t(sctp);
	}

	static const int kRawValue = 600;
	static const int kFileValue = 700;

	const LEVELS
		RAW_GREEN_TO_CONSOLE{ kRawValue, {"RAW_GREEN_TO_CONSOLE"} },
		RAW_RED{ kRawValue, {"RAW_RED"} },
		INFO_TO_FILE{ kFileValue , {"INFO_TO_FILE"} };

	class logger;
	inline logger* g_log{};

	enum class LogColor
	{
		RESET,
		WHITE = 97,
		CYAN = 36,
		MAGENTA = 35,
		BLUE = 34,
		GREEN = 32,
		YELLOW = 33,
		RED = 31,
		BLACK = 30
	};

#define AddColorToStream(color) "\x1b[" << int(color) << "m"
#define ResetStreamColor "\x1b[" << int(LogColor::RESET) << "m"
#define HEX_TO_UPPER(value) "0x" << std::hex << std::uppercase << (DWORD64)value << std::dec << std::nouppercase

	class logger final
	{

	public:
		logger(std::string_view console_title, file file, bool attach_console = true)
			: m_attach_console(attach_console), m_did_console_exist(false),
			m_console_title(console_title), m_original_console_mode(0),
			m_console_handle(nullptr), m_file(file),
			m_worker(g3::LogWorker::createLogWorker()), m_attach_target(GetCurrentProcessId())
		{
			initialize();

			g_log = this;
		}
		~logger()
		{
			g_log = nullptr;
		}

		void initialize()
		{
			if (m_attach_console)
			{
				if (m_did_console_exist = ::AttachConsole(m_attach_target); !m_did_console_exist)
					AllocConsole();

				if (m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE); m_console_handle != nullptr)
				{
					SetConsoleTitleA(m_console_title.data());
					SetConsoleOutputCP(CP_UTF8);

					DWORD console_mode;
					GetConsoleMode(m_console_handle, &console_mode);
					m_original_console_mode = console_mode;

					// terminal like behaviour enable full color support
					console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
					// prevent clicking in terminal from suspending our main thread
					console_mode &= ~(ENABLE_QUICK_EDIT_MODE);

					SetConsoleMode(m_console_handle, console_mode);
				}
			}
			create_backup();
			open_outstreams();
			initialize_g3log();
		}
		void destroy()
		{
			destroy_g3log();
			close_outstreams();

			if (m_did_console_exist)
				SetConsoleMode(m_console_handle, m_original_console_mode);

			if (!m_did_console_exist && m_attach_console)
				FreeConsole();
		}

	private:
		void create_backup()
		{
			if (m_file.exists())
			{
				auto file_time = std::filesystem::last_write_time(m_file.get_path());
				auto time_t = to_time_t(file_time);
				auto local_time = std::localtime(&time_t);

				m_file.move(
					std::format(
						"./backup/{:0>2}-{:0>2}-{}-{:0>2}-{:0>2}-{:0>2}_{}",
						local_time->tm_mon + 1,
						local_time->tm_mday,
						local_time->tm_year + 1900,
						local_time->tm_hour,
						local_time->tm_min,
						local_time->tm_sec,
						m_file.get_path().filename().string().c_str()
					)
				);
			}
		}

		void initialize_g3log()
		{
			m_worker->addSink(std::make_unique<log_sink>(), &log_sink::callback);
			g3::initializeLogging(m_worker.get());
		}
		void destroy_g3log()
		{
			m_worker->removeAllSinks();
			m_worker.reset();
		}

		void open_outstreams()
		{
			if (m_attach_console)
				m_console_out.open("CONOUT$", std::ios_base::out | std::ios_base::app);

			m_file_out.open(m_file.get_path(), std::ios_base::out | std::ios_base::trunc);
		}
		void close_outstreams()
		{
			if (m_attach_console)
				m_console_out.close();

			m_file_out.close();
		}

	private:
		struct log_sink
		{
			void callback(g3::LogMessageMover log)
			{
				g3::LogMessage log_message = log.get();
				int level_value = log_message._level.value;

				bool is_raw = level_value == RAW_GREEN_TO_CONSOLE.value;
				bool is_file = level_value == INFO_TO_FILE.value;

				if (g_log->m_console_out.is_open() && !is_file)
					g_log->m_console_out << log_message.toString(is_raw ? log_sink::format_raw : log_sink::format_console) << std::flush;

				if (!is_raw || is_file)
					g_log->m_file_out << log.get().toString(log_sink::format_file) << std::flush;
			}

			static LogColor get_color(const LEVELS level)
			{
				switch (level.value)
				{
				case g3::kDebugValue:
					return LogColor::BLUE;
				case g3::kInfoValue:
					return LogColor::GREEN;
				case g3::kWarningValue:
					return LogColor::YELLOW;
				case kRawValue:
					return LogColor::GREEN;
				}
				return g3::internal::wasFatal(level) ? LogColor::RED : LogColor::WHITE;
			}

			static std::string format_console(const g3::LogMessage& msg)
			{
				LogColor color = log_sink::get_color(msg._level);
				std::stringstream out;

				out
					<< "[" << msg.timestamp("%H:%M:%S") << "]"
					<< AddColorToStream(color)
					<< "[" << msg.level() << "]"
					<< "[" << msg.file() << ":" << msg.line() <<"]"
					<< ": ";

				return out.str();
			}
			static std::string format_raw(const g3::LogMessage& msg)
			{
				LogColor color = log_sink::get_color(msg._level);
				std::stringstream out;
				out << AddColorToStream(color);

				return out.str();
			}
			static std::string format_file(const g3::LogMessage& msg)
			{
				LogColor color = log_sink::get_color(msg._level);
				std::stringstream out;//<< std::setw(7) 

				out
					<< "[" << msg.timestamp("%H:%M:%S") << "]"
					<< "[" << msg.level() << "]"
					<< "[" << msg.file() << "]" << "[" << msg.line() << "]"
					<< ": ";

				return out.str();
			}
		};

	private:
		friend struct log_sink;

		bool m_attach_console;
		bool m_did_console_exist;

		std::string_view m_console_title;
		DWORD m_original_console_mode;
		DWORD m_attach_target;
		HANDLE m_console_handle;

		std::ofstream m_console_out;
		std::ofstream m_file_out;

		file m_file;

		std::unique_ptr<g3::LogWorker> m_worker;

	};
}