#pragma once

namespace common {
	class Counter {
	public:
		static int get_current_number();
	private:
		static int count;
	};
}