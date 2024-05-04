#pragma once
#include "common.hpp"
#define CHUNK_SIZE 0x1000

namespace ellohim 
{

	class pattern 
	{
		std::vector<std::optional<uint8_t>> compiled;
		std::string name;
		uintptr_t   address = 0;
		uintptr_t   m_base_address = 0;

	public:
		explicit pattern(std::string name, std::string x);
		explicit pattern(std::string name_, uintptr_t addr);

		void compile(const std::string& pat);

		pattern& scan();

		inline pattern add(size_t n)
		{
			return pattern(name, address + n);
		}

		inline pattern sub(size_t n) 
		{
			return pattern(name, address - n);
		}

		pattern rip();

		inline operator std::uint64_t() const
		{
			return address;
		}
	};
};