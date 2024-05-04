#pragma once
#include "common.hpp"

namespace ellohim
{
	class process;
	inline process* g_process;

	class process
	{
		HANDLE m_handle;
		uint32_t m_pid;

	public:
		uintptr_t m_base_address;
		size_t m_size;

		process(uint32_t pid) :
			m_pid(pid)
		{
			m_handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			if (!m_handle)
			{
				throw std::exception("Failed to open handle");
			}

			HMODULE modules[0xFF];
			MODULEINFO module_info;
			DWORD _;

			if (!::EnumProcessModulesEx(m_handle, modules, sizeof(modules), &_, LIST_MODULES_64BIT))
				throw std::exception("Failed to enumerate modules");

			m_base_address = (uintptr_t)modules[0];

			if (!::GetModuleInformation(m_handle, modules[0], &module_info, sizeof(module_info)))
				throw std::exception("Failed to get module info");

			m_size = module_info.SizeOfImage;

			g_process = this;
		}

		~process()
		{
			::CloseHandle(m_handle);
			g_process = nullptr;
		}

		inline bool is_running()
		{
			uint32_t exit_code;
			if (::GetExitCodeProcess(m_handle, (LPDWORD)&exit_code))
			{
				return exit_code == STATUS_PENDING;
			}
			else 
			{
				return false;
			}
		}

		inline void set_paused(bool paused)
		{
			if (paused)
			{
				::SuspendThread(m_handle);
			}
			else
			{
				::ResumeThread(m_handle);
			}
		}

		inline bool read_raw_ex(uintptr_t base, size_t size, void* out)
		{
			return ::ReadProcessMemory(m_handle, (LPCVOID)base, out, size, nullptr);
		}

		inline void read_raw(uintptr_t base, size_t size, void* out)
		{
			if (!::ReadProcessMemory(m_handle, (LPCVOID)base, out, size, nullptr))
			{
				throw std::exception("RPM failed");
			}
		}

		inline void write_raw(uintptr_t base, size_t size, void* buf)
		{
			if (!::WriteProcessMemory(m_handle, (LPVOID)base, buf, size, nullptr))
			{
				throw std::exception("RPM failed");
			}
		}

		inline void read_char(uintptr_t addr, char* dst)
		{

			unsigned int arraySize;
			//Get the size of the array, offset 0x14 is the size of the array
			ReadProcessMemory(m_handle, (BYTE*)(addr + 0x14), &arraySize, sizeof(arraySize), 0);

			if (arraySize > 15)
			{
				uintptr_t addrOfCharArray;
				//dereference the pointer in the second member variable to get the dynamic address of the array
				ReadProcessMemory(m_handle, (BYTE*)(addr + sizeof(void*)), &addrOfCharArray, sizeof(void*), 0);

				char buffer[256];
				//Read the array into buffer, +1 to get the null terminator
				ReadProcessMemory(m_handle, (BYTE*)(addrOfCharArray), &buffer, arraySize + 1, 0);

				//copy the buffer into our ouput argument
				memcpy(dst, &buffer, strlen(buffer) + 1);
			}
			else
			{
				ReadProcessMemory(m_handle, (BYTE*)(addr + sizeof(void*)), dst, arraySize, 0);
			}
		}

		//not recommended
		inline std::string read_string(uintptr_t ptr)
		{
			std::string str = "NULL";
			str.resize(64);

			read_raw(ptr, sizeof(ptr), str.data());

			return str;
		}

		template <typename T>
		inline void read(uintptr_t base, T& ref)
		{
			read_raw(base, sizeof(T), &ref);
		}

		template <typename T>
		inline T read(uintptr_t base)
		{
			T out{};
			read_raw(base, sizeof(T), &out);
			return out;
		}

		template <typename T>
		inline void write(uintptr_t base, const T& ref) 
		{
			write_raw(base, sizeof(T), (void*)&ref);
		}

		inline uint64_t allocate(size_t size)
		{
			return (uint64_t)::VirtualAllocEx(m_handle, nullptr, size, MEM_COMMIT, PAGE_READWRITE);
		}
	};
}