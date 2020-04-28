#pragma once

#include "audio-buffer.hpp"

class AudioSource
{
public:
  enum class State
  {
    PLAYING,
    STOPPED
  };

  AudioSource(const AudioBuffer &audio_buffer);

  ~AudioSource();

  void play() const;

  State get_state() const;

private:
  ALuint id = 0;

  State al_state_to_state(ALint state) const;
};
