#include "common_algorithm.h"

namespace common {
	int Counter::get_current_number() {
		int c = count;
		count++;
		return c;
	}
}