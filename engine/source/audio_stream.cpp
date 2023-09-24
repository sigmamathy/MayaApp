#include <Maya/audio_stream.hpp>
#include <portaudio.h>
#include <sndfile.h>

namespace Maya {

static int AudioCallback(const void* input_buffer, void* output_buffer,
                                   unsigned long frames_per_buffer,
                                   const PaStreamCallbackTimeInfo* time_info,
                                   PaStreamCallbackFlags status_flags,
                                   void* user_data)
{
    float* out = (float*) output_buffer;
    AudioStream::DataInfo* status = (AudioStream::DataInfo*) user_data;
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
    SF_INFO sf_info;
    SNDFILE* file = sf_open(filepath.c_str(), SFM_READ, &sf_info);
#if MAYA_DEBUG
    if (!file) {
        MAYA_LOG("Error (AudioStream::AudioStream): Cannot open file \"" << filepath << "\"");
        return;
    }
#endif
    samples.resize(sf_info.frames * sf_info.channels);
    sf_readf_float(file, samples.data(), sf_info.frames);
    sf_close(file);

    info.samples = samples.data();
    info.total_frames = sf_info.frames;
    info.frames_per_buffer = frames_per_buffer;
    info.sample_rate = sf_info.samplerate;
    info.channels = sf_info.channels;
    info.duration = (float)sf_info.frames / sf_info.samplerate;
    info.is_playing = false;
    info.current_sample_position = 0;
    info.replay_at_end = false;
    info.time = 0;
    info.volume = 1.0f;

    // Set up the audio stream parameters
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = info.channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Open a stream with PortAudio
    PaStream* stream;
    Pa_OpenStream(&stream, NULL, &outputParameters, info.sample_rate, frames_per_buffer,
        paNoFlag, AudioCallback, 0);
    this->stream = stream;
}

AudioStream::~AudioStream()
{
    Pa_CloseStream((PaStream*)stream);
}

void AudioStream::Start(bool repeat)
{
    info.is_playing = true;
    info.current_sample_position = 0;
    info.replay_at_end = repeat;
    info.time = 0;
    Pa_StartStream((PaStream*)stream);
}

void AudioStream::Stop()
{
    info.is_playing = false;
    Pa_StopStream((PaStream*)stream);
}

void AudioStream::SetVolume(float volume)
{
    info.volume = volume;
}

}