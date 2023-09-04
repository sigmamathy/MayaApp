#pragma once

#include "./math.hpp"

namespace Maya {

class AudioStream
{
public:
	AudioStream(std::string const& filepath, unsigned int frame_per_buffer = 256);

	void Play(bool repeat = false);
	void Stop();

private:
	void* stream;
	void* audio_play_info;

private:
	~AudioStream();
	AudioStream(AudioStream const&) = delete;
	AudioStream& operator=(AudioStream const&) = delete;
	friend class PrivateControl;
};

}