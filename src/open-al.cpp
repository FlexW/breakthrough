#include "open-al.hpp"

bool check_al_errors(const std::string &filename, const std::uint_fast32_t line)
{
  static const std::string LOG_TAG = "OpenAl";

  ALenum error = alGetError();

  if (error != AL_NO_ERROR)
  {
    const std::string error_prefix =
        filename + ": " + std::to_string(line) + " ";

    switch (error)
    {
    case AL_INVALID_NAME:
      Log().e(LOG_TAG)
          << error_prefix
          << "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL "
             "function";
      break;
    case AL_INVALID_ENUM:
      Log().e(LOG_TAG)
          << error_prefix
          << "AL_INVALID_ENUM: an invalid enum value was passed to an "
             "OpenAL function";
      break;
    case AL_INVALID_VALUE:
      Log().e(LOG_TAG)
          << error_prefix
          << "AL_INVALID_VALUE: an invalid value was passed to an OpenAL "
             "function";
      break;
    case AL_INVALID_OPERATION:
      Log().e(LOG_TAG)
          << error_prefix
          << "AL_INVALID_OPERATION: the requested operation is not valid";
      break;
    case AL_OUT_OF_MEMORY:
      Log().e(LOG_TAG)
          << error_prefix
          << "AL_OUT_OF_MEMORY: the requested operation resulted in "
             "OpenAL running out of memory";
      break;
    default:
      Log().e(LOG_TAG) << error_prefix << "UNKNOWN AL ERROR: " << error;
    }
    return false;
  }
  return true;
}

bool check_alc_errors(const std::string &      filename,
                      const std::uint_fast32_t line,
                      ALCdevice *              device)
{
  static const std::string LOG_TAG = "OpenAl";

  ALCenum error = alcGetError(device);

  if (error != ALC_NO_ERROR)
  {
    const std::string error_prefix =
        filename + ": " + std::to_string(line) + " ";

    switch (error)
    {
    case ALC_INVALID_VALUE:
      Log().e(LOG_TAG)
          << error_prefix
          << "ALC_INVALID_VALUE: an invalid value was passed to an "
             "OpenAL function";
      break;
    case ALC_INVALID_DEVICE:
      Log().e(LOG_TAG)
          << error_prefix
          << "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL "
             "function";
      break;
    case ALC_INVALID_CONTEXT:
      Log().e(LOG_TAG)
          << error_prefix
          << "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL "
             "function";
      break;
    case ALC_INVALID_ENUM:
      Log().e(LOG_TAG)
          << error_prefix
          << "ALC_INVALID_ENUM: an unknown enum value was passed to an "
             "OpenAL function";
      break;
    case ALC_OUT_OF_MEMORY:
      Log().e(LOG_TAG)
          << error_prefix
          << "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an "
             "OpenAL function";
      break;
    default:
      Log().e(LOG_TAG) << error_prefix << "UNKNOWN ALC ERROR: " << error;
    }
    return false;
  }
  return true;
}
