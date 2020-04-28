#include "audio-buffer.hpp"
#include "asseration.hpp"
#include "log.hpp"

static const std::string LOG_TAG = "AudioBuffer";

AudioBuffer::AudioBuffer(const Format                      format,
                         const std::vector<unsigned char> &data,
                         const std::int32_t                sample_rate,
                         const std::size_t                 size)
{
  AL_CALL(alGenBuffers, 1, &id);
  AL_CALL(alBufferData,
          id,
          format_to_al_enum(format),
          data.data(),
          size,
          sample_rate);
}

AudioBuffer::~AudioBuffer()
{
  Log().d(LOG_TAG) << "Delete audio buffer with id " << id;
  AL_CALL(alDeleteBuffers, 1, &id);
}

ALenum AudioBuffer::format_to_al_enum(Format format)
{
  switch (format)
  {
  case Format::MONO8:
    return AL_FORMAT_MONO8;
  case Format::MONO16:
    return AL_FORMAT_MONO16;
  case Format::STEREO8:
    return AL_FORMAT_STEREO8;
  case Format::STEREO16:
    return AL_FORMAT_STEREO16;
  default:
    FAIL;
  }
}
