#pragma once
#include <vector>
#include <string>
#include "libavformat/avformat.h"
#include <functional>
#include <set>
#include "timeline.h"

/*Namespace containing every classes that are concerned with generating */
namespace data_generation {
	class MediaStream {
	public:
		AVCodecContext* context;
		std::vector<std::function<void(long, AVFrame*)>> transformers;
		long curr_time;
		long duration;

		MediaStream();
		MediaStream(AVCodecContext*, long);
		~MediaStream();

		void read_packet(AVPacket* packet, AVFrame* frame);
		void add_transformer(std::function<void(long, AVFrame*)>);

	private:
		void apply_transformations(AVFrame*);
		void update_curr_time();
	};

	class MediaFile : public TimelineObject {
	public:
		AVFormatContext* context;
		std::vector<MediaStream> streams;
		std::vector<double> progress;
		AVPacket* curr_packet;

		MediaFile(long, std::string);
		~MediaFile();

		void initialize_context(std::string file_name);
		void generate_next_frame(AVFrame*);
 	};
}