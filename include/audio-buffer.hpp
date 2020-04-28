#pragma once

#include <vector>

#include "open-al.hpp"

class AudioBuffer
{
public:
  enum class Format
  {
    MONO8,
    MONO16,
    STEREO8,
    STEREO16
  };

  AudioBuffer(const Format                      format,
              const std::vector<unsigned char> &data,
              const std::int32_t                sample_rate,
              const std::size_t                 size);

  ~AudioBuffer();

  unsigned get_id() const { return reinterpret_cast<unsigned>(id); }

private:
  ALuint id = 0;

  ALenum format_to_al_enum(Format format);
};
