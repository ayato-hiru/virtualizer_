#include "logger.h"

namespace common {
	void Logger::add_error(std::string error_message) {
		Logger::has_error = true;

		Logger::errors.insert(Logger::errors.begin(), std::tuple<std::string, std::string> { "[ERROR]: ", error_message });
	}

	void Logger::add_warning(std::string warning_message) {
		Logger::errors.insert(Logger::errors.begin(), std::tuple<std::string, std::string> { "[WARNING]: ", warning_message });
	}

	void Logger::print_messages() {
		for (auto &msg : Logger::errors) {
			std::stringstream s;
			std::string type = std::get<0>(msg);
			std::string message = std::get<1>(msg);
			s << type << message;

			std::cout << s.str() << std::endl;
		}
	}

	bool Logger::check_error() {
		return has_error;
	}

	void Logger::clear_error() {
		Logger::has_error = false;
	}
}