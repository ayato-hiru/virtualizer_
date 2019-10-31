#pragma once

namespace common {
	enum Status {
		SUCCESS,
		ERROR
	};

	enum ErrorType {
		MEDIA_READING_ERROR,
		/*Not an error type; used to keep track of the length of error so WarningType can begin from the end*/
		LENGTH
	};

	enum WarningType {
		GENERIC_WARNING = ErrorType::LENGTH
	};

	union MessageType {
		ErrorType err;
		WarningType warn;
	};
}