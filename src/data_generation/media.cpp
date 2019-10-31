#include "media.h"
#include "common/logger.h"
#include "common/common_algorithm.h"
#include <sstream>


namespace data_struct {
	MediaFile::MediaFile(long start_time, std::string file_name) {
		// initialize AVFormatContext using the file given
		initialize_context(file_name);
		this->start_time = start_time;
		this->complete = false;
	}

	MediaFile::~MediaFile() {
		avformat_free_context(this->context);
	}

	/*Initialize AVFormatContext used for this media file.*/
	void MediaFile::initialize_context(std::string dir) {
		// get the name of the file for logging purposes
		auto name_ind = dir.find_last_of("\\/");
		std::string file_name = dir;
		file_name.erase(0, name_ind + 1);

		// read the file
		AVFormatContext* context = NULL;
		if (avformat_open_input(&context, dir.c_str(), NULL, NULL) < 0) {
			common::Logger::add_error("Could not read file \"" + file_name + "\"");
			return;
		}

		// get streams
		if (avformat_find_stream_info(context, NULL) < 0) {
			common::Logger::add_error("Could not read streams for file \"" + file_name + "\"");
			return;
		}

		// loop through all the streams, and find appropriate Codec Context for it, initialize MediaStream and store it in list
		for (unsigned int i = 0; i < context->nb_streams; i++) {
			AVCodecParameters* param = context->streams[i]->codecpar;

			// only add streams if it's supported (audio or video).  May be expanded later on
			if (param->codec_type == AVMEDIA_TYPE_AUDIO || param->codec_type == AVMEDIA_TYPE_VIDEO) {
				AVCodec* codec = avcodec_find_decoder(param->codec_id);
				AVCodecContext* codec_context = avcodec_alloc_context3(codec);

				avcodec_parameters_to_context(codec_context, param);
				avcodec_open2(codec_context, codec, NULL);

				this->streams.push_back(MediaStream(codec_context, context->streams[i]->duration));
				this->progress.push_back(0.0);
			}
			// if the stream is something not supported, add NULL to the vector instead to prevent the index from offsetting.
			else {
				this->streams.push_back(MediaStream());
				this->progress.push_back(0);
			}
		}

		this->context = context;
	}

	void MediaFile::generate_next_frame(AVFrame* frame) {
		
	}

	MediaStream::MediaStream() {
		this->context = nullptr;
		this->curr_time = 0;
		this->duration = 0;
	}

	MediaStream::MediaStream(AVCodecContext* ctx, long duration) {
		this->context = ctx;
		this->curr_time = 0;
		this->duration = duration;
	}

	MediaStream::~MediaStream() {
		avcodec_free_context(&this->context);
	}

	/*Read the given packet and store it in frame*/
	void MediaStream::read_packet(AVPacket* packet, AVFrame* frame) {
		// Send the packet with the context.  If there's any error, return.
		if (avcodec_send_packet(this->context, packet) < 0) {
			common::Logger::add_error("Error while reading packet.");
			return;
		}

		// Read the frame
		int resp = avcodec_receive_frame(this->context, frame);

		// valid response: EOF, packet not enough for frame, or success.  Otherwise, there was an error.
		if (resp < 0) {
			common::Logger::add_error("Error while receiving frame.");
			return;
		}

		// if the frame wasn't successfully retrieved from the packet, don't use the frame yet
		if (resp == AVERROR_EOF || resp == AVERROR(EAGAIN)) {
			return;
		}

		// apply the transformation to frame before returning
		apply_transformations(frame);

		// update the current time, TODO: figure out how to do it effectively
		update_curr_time();
	}

	void MediaStream::add_transformer(std::function<void(long, AVFrame*)> t) {
		this->transformers.push_back(t);
	}

	void MediaStream::apply_transformations(AVFrame* frame) {
		for (auto transformer : this->transformers) {
			transformer(this->curr_time, frame);
		}
	}

	/*Updates the current time the stream is on*/
	void MediaStream::update_curr_time() {
		auto codec_type = this->context->codec_type;
		// If the codec type is video, increase time based on fps
		if (codec_type == AVMEDIA_TYPE_VIDEO) {

		}

		// If the codec type is audio, update based on sample rate.
		else if (codec_type == AVMEDIA_TYPE_AUDIO) {
			// current time can be calculated by figuring out by ((current_frame) / (total_frame)) * total_duration
			
			this->curr_time = (long)floor((double)(this->context->frame_number) / ((long)(this->context->frame_size) * (long)(this->context->channels)) * (this->duration));
		}

		// If the codec type is anything else, somehow a codec not supported is added.  Error
		else {
			common::Logger::add_error("Failed to calculate duration for this codec type.");
		}
	}
}