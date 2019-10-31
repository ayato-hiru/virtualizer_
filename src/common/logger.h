#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "enum_definition.h"
#include <sstream>
#include <tuple>

namespace common {
	class Logger {
	private:
		static std::vector<std::tuple<std::string, std::string>> errors;
		static bool has_error;
	public:
		static void add_error(std::string);
		static void add_warning(std::string);
		static void print_messages();
		static bool check_error();
		static void clear_error();
	};
}