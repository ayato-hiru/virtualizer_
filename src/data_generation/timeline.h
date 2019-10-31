#pragma once
#include <set>
#include <vector>
#include <algorithm>
#include "libavformat/avformat.h"
#include <queue>

namespace data_generation {
	class TimelineObject {
	public:
		bool operator <(const TimelineObject&) const;
		bool complete;
		long start_time;
	};

	class Timeline {
	public:
		Timeline();

		void add(TimelineObject);
		void play(long);
		void generate();
		void get_config(int&, int&);
		void find_currently_playing(std::multiset<TimelineObject>::iterator&, std::multiset<TimelineObject>::iterator, std::vector<TimelineObject>&, long);
	private:
		bool comp(TimelineObject obj1, TimelineObject obj2) {
			return obj1.start_time < obj2.start_time;
		}
		std::multiset <TimelineObject> objects;
	};
}