#pragma once
#include <string>
#include <sstream>

#include "include/fmt/color.h"

namespace utils {
	inline int StrToInt(const std::string& str) {
		std::stringstream ss;
		auto result = 0;
		
		ss << str;
		ss >> result;

		return ss.fail() ? -1 : result;
	}

	inline std::string GetDelimiter(const char symbol, const size_t size) {
		return std::string(size, symbol);
	}

	inline void ConsoleClear() {
		fmt::print(fg(fmt::color::white), "\n");
		#if defined(_WIN32)
			system("cls");
		#else
			system("clear");
		#endif
	}

	inline void ConsolePause() {
		#if defined(_WIN32)
			system("pause");
		#endif
	}
}
