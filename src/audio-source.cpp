#include "audio-source.hpp"
#include "asseration.hpp"

static const std::string LOG_TAG = "AudioSource";

AudioSource::AudioSource(const AudioBuffer &audio_buffer)
{
  AL_CALL(alGenSources, 1, &id);
  AL_CALL(alSourcef, id, AL_PITCH, 1);
  AL_CALL(alSourcef, id, AL_GAIN, 1.0f);
  AL_CALL(alSource3f, id, AL_POSITION, 0, 0, 0);
  AL_CALL(alSource3f, id, AL_VELOCITY, 0, 0, 0);
  AL_CALL(alSourcei, id, AL_LOOPING, AL_FALSE);
  AL_CALL(alSourcei, id, AL_BUFFER, audio_buffer.get_id());
}

AudioSource::~AudioSource()
{
  Log().d(LOG_TAG) << "Delete audio source with id " << id;
  AL_CALL(alDeleteSources, 1, &id);
}

void AudioSource::play() const { AL_CALL(alSourcePlay, id); }

AudioSource::State AudioSource::get_state() const
{
  ALint state;
  AL_CALL(alGetSourcei, id, AL_SOURCE_STATE, &state);
  return al_state_to_state(state);
}

AudioSource::State AudioSource::al_state_to_state(ALint state) const
{
  switch (state)
  {
  case AL_PLAYING:
    return State::PLAYING;
  case AL_STOPPED:
    return State::STOPPED;
  default:
    FAIL;
  }
}
