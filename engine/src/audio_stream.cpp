#include "./private_control.hpp"
#include <sndfile.h>

namespace Maya {

static int PortAudio_audioCallback(const void* input_buffer, void* output_buffer,
                                   unsigned long frames_per_buffer,
                                   const PaStreamCallbackTimeInfo* time_info,
                                   PaStreamCallbackFlags status_flags,
                                   void* user_data)
{
    float* out = (float*) output_buffer;
    AudioStreamStatus* status = (AudioStreamStatus*) user_data;
    unsigned int remaining_samples = status->total_frames * status->channels - status->current_sample_position;
    status->time = (float) status->current_sample_position / status->sample_rate / status->channels;

    if (remaining_samples >= frames_per_buffer * status->channels)
    {
        for (unsigned int i = 0; i < frames_per_buffer * status->channels; i++)
            out[i] = status->samples[status->current_sample_position++] * status->volume;
        return paContinue;
    }

    for (unsigned int i = 0; i < remaining_samples; i++)
        out[i] = status->samples[status->current_sample_position++] * status->volume;
    
    if (!status->replay_at_end) 
        return paComplete;

    status->current_sample_position = 0;
    return paContinue;
}

AudioStream::AudioStream(std::string const& filepath, unsigned int frames_per_buffer)
{
    SF_INFO info;
    SNDFILE* file = sf_open(filepath.c_str(), SFM_READ, &info);
#if MAYA_DEBUG
    if (!file) {
        std::cout << "Failed to open the audio file.\n";
        return;
    }
#endif
    samples.resize(info.frames * info.channels);
    sf_readf_float(file, samples.data(), info.frames);
    sf_close(file);

    status.samples                  = samples.data();
    status.total_frames             = info.frames;
    status.frames_per_buffer        = frames_per_buffer;
    status.sample_rate              = info.samplerate;
    status.channels                 = info.channels;
    status.duration                 = (float)info.frames / info.samplerate;
    status.is_playing               = false;
    status.current_sample_position  = 0;
    status.replay_at_end            = false;
    status.time                     = 0;
    status.volume                   = 1.0f;

    // Set up the audio stream parameters
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = info.channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Open a stream with PortAudio
    PaStream* stream;
    Pa_OpenStream(&stream, NULL, &outputParameters, info.samplerate, frames_per_buffer,
        paNoFlag, PortAudio_audioCallback, &status);
    this->stream = stream;
}

AudioStream::~AudioStream()
{
    Pa_CloseStream((PaStream*)stream);
}

void AudioStream::Start(bool repeat)
{
    status.is_playing               = true;
    status.current_sample_position  = 0;
    status.replay_at_end            = repeat;
    status.time                     = 0;
    Pa_StartStream((PaStream*)stream);
}

void AudioStream::Stop()
{
    status.is_playing = false;
    Pa_StopStream((PaStream*)stream);
}

void AudioStream::SetVolume(float volume)
{
    status.volume = volume;
}

AudioStreamStatus const& AudioStream::GetAudioStreamStatus() const
{
    return status;
}

}