#include "timeline.h"

namespace data_generation {
	Timeline::Timeline() {}

	void Timeline::add(TimelineObject object) {
		this->objects.insert(object);
	}

	void Timeline::play(long) {

	}

	void Timeline::generate() {
		long curr_time = 0;
		auto it = this->objects.begin();
		std::vector<TimelineObject> currently_playing;
		long frame_video = 0, frame_audio = 0;
		int fps, sample_rate;

		get_config(fps, sample_rate);

		// the buffer queue used to store generated audio and video
		std::queue<std::tuple<long, AVFrame*>> generated_audio;
		std::queue<std::tuple<long, AVFrame*>> generated_video;

		// the buffer used to wait for every relevant frame to be added
		std::vector<std::tuple<long, AVFrame*>> combine_audio;
		std::vector<std::tuple<long, AVFrame*>> combine_video;

		// the buffer used to tell which action should be prioritized.
		// 1: generate audio
		// 2: generate video
		// 3: combine audio into one
		// 4: combine video into one
		// 5: push action based on what seems to be lacking
		// 0: no more action
		std::queue<int> action_type;
		action_type.push(1);

		int action;

		// repeat until the end action has been pushed
		while (action = action_type.front() != 0) {
			// consider this action done
			action_type.pop();

			// decide which action to perform based on the type
			switch (action) {
				// generate audio
				case 1: {

					break;
				}
			}
		}

		find_currently_playing(it, this->objects.end(), currently_playing, curr_time);

		long next_video = (frame_video / fps) * 1000;
		long next_audio = (frame_audio / sample_rate);
	}

	void Timeline::find_currently_playing(std::multiset<TimelineObject>::iterator &it, std::multiset<TimelineObject>::iterator end, std::vector<TimelineObject> &currently_playing, long curr_time) {
		// add new data
		while (it != end) {
			// if the timeline object is supposed to be playing, add it to the list
			if (it->start_time < curr_time) {
				currently_playing.push_back(*it);
				it++;
			}
			// otherwise, since the set is in order, the others will not be playing.  break the loop
			else {
				break;
			}
		}

		// remove data that already ended
		std::remove_if(currently_playing.begin(), currently_playing.end(), [curr_time](TimelineObject obj) {
			return obj.complete;
		});
	}

	/*Placeholder*/
	void Timeline::get_config(int& fps, int& sample_rate) {
		fps = 60;
		sample_rate = 44100;
	}

	bool TimelineObject::operator<(const TimelineObject& obj) const {
		return this->start_time < obj.start_time;
	}
}