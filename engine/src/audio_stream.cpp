#include "./private_control.hpp"
#include <sndfile.h>

namespace Maya {

struct AudioPlayInfo
{
    std::vector<float> data;
    int channels;
    unsigned int total_samples;
    unsigned int current_position;
    float volume;
    bool replay;
};

static int PortAudio_audioCallback(const void* input_buffer, void* output_buffer,
                                   unsigned long frames_per_buffer,
                                   const PaStreamCallbackTimeInfo* time_info,
                                   PaStreamCallbackFlags status_flags,
                                   void* user_data)
{
    float* out = (float*)output_buffer;
    AudioPlayInfo* audio_play_info = (AudioPlayInfo*)user_data;
    unsigned long remainingSamples = audio_play_info->total_samples - audio_play_info->current_position;

    if (remainingSamples >= frames_per_buffer * audio_play_info->channels) {
        // More samples available, copy them to the output buffer
        for (unsigned long i = 0; i < frames_per_buffer; i++) {
            for (int channel = 0; channel < audio_play_info->channels; channel++) {
                *(out++) = audio_play_info->data[audio_play_info->current_position++] * audio_play_info->volume;
            }
        }
    }
    else {
        // Reached the end of the audio data
        if (audio_play_info->replay) {
            // Restart playback from the beginning
            audio_play_info->current_position = 0;
            return paContinue;
        }
        else {
            // Output silence to complete the buffer
            for (unsigned long i = 0; i < frames_per_buffer; i++) {
                for (int channel = 0; channel < audio_play_info->channels; channel++) {
                    *(out++) = 0.0f;
                }
            }
            return paComplete;
        }
    }

    return paContinue;
}

AudioStream::AudioStream(std::string const& filepath, unsigned int frame_per_buffer)
{
    SF_INFO fileInfo;
    SNDFILE* file = sf_open(filepath.c_str(), SFM_READ, &fileInfo);
#if MAYA_DEBUG
    if (!file) {
        std::cout << "Failed to open the audio file.\n";
        return;
    }
#endif
    AudioPlayInfo* api = new AudioPlayInfo;
    api->data.resize(fileInfo.frames * fileInfo.channels);
    sf_readf_float(file, api->data.data(), fileInfo.frames);
    sf_close(file);
    api->channels = fileInfo.channels;
    api->total_samples = api->data.size();
    api->current_position = 0;
    api->volume = 1.0f;
    audio_play_info = api;

    // Set up the audio stream parameters
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = fileInfo.channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Open a stream with PortAudio
    PaStream* stream;
    Pa_OpenStream(&stream, NULL, &outputParameters, fileInfo.samplerate, frame_per_buffer,
        paNoFlag, PortAudio_audioCallback, audio_play_info);
    this->stream = stream;
}

AudioStream::~AudioStream()
{
    Pa_CloseStream((PaStream*)stream);
    delete (AudioPlayInfo*) audio_play_info;
}

void AudioStream::Play(bool repeat)
{
    auto* api = (AudioPlayInfo*) audio_play_info;
    api->current_position = 0;
    api->replay = repeat;
    Pa_StartStream((PaStream*)stream);
}

void AudioStream::Stop()
{
    Pa_StopStream((PaStream*)stream);
}

void AudioStream::SetVolume(float volume)
{
    auto* api = (AudioPlayInfo*)audio_play_info;
    api->volume = volume;
}

}