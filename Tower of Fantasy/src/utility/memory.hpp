#pragma once
#include "memory/process.hpp"

namespace ellohim::memory
{
	
    inline uintptr_t readPointer(uintptr_t addr, std::vector<uint32_t> offsets)
	{
        auto ptr = g_process->read<uintptr_t>(addr);

        if (!ptr) return NULL;

        auto level = offsets.size();

        for (auto i = 0; i < level; i++)
        {
            if (i == level - 1)
            {
                ptr += offsets[i];
                if (!ptr) return NULL;
                return ptr;
            }
            else
            {
                ptr = g_process->read<uintptr_t>(ptr + offsets[i]);
                if (!ptr) return NULL;
            }
        }
        return ptr;
	}

    template<typename T>
    T read(uintptr_t addr, std::vector<uint32_t> offsets = {})
    {
        if (uintptr_t address = readPointer(addr, offsets))
            return g_process->read<T>(address);

        return NULL;
    }

    template<typename T>
    void write(uintptr_t address, std::vector<uint32_t> offsets, T value)
    {
        if (auto addr = readPointer(address, offsets))
            g_process->write(addr, value);
    }
}