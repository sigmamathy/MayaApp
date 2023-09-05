#pragma once

#include "./math.hpp"

namespace Maya {

struct AudioStreamStatus
{
	float const* samples;
	unsigned int total_frames;
	unsigned int frames_per_buffer;
	int sample_rate;
	int channels;
	float duration;

	bool is_playing;
	unsigned int current_sample_position;
	bool replay_at_end;
	float time;

	float volume;
};

class AudioStream
{
public:
	AudioStream(std::string const& filepath, unsigned int frame_per_buffer = 256);

	void Start(bool repeat = false);
	void Stop();

	void SetVolume(float volume);
	AudioStreamStatus const& GetAudioStreamStatus() const;

private:
	void* stream;
	std::vector<float> samples;
	AudioStreamStatus status;

private:
	~AudioStream();
	AudioStream(AudioStream const&) = delete;
	AudioStream& operator=(AudioStream const&) = delete;
	friend class PrivateControl;
};

}