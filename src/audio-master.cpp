#include "audio-master.hpp"

static const std::string LOG_TAG = "AudioMaster";

AudioMaster::AudioMaster()
{
  // Open default device
  alc_device = alcOpenDevice(nullptr);
  if (!alc_device)
  {
    throw std::runtime_error("Could not open default audio device");
  }

  // Load context
  if (!ALC_CALL(alcCreateContext,
                alc_context,
                alc_device,
                alc_device,
                nullptr) ||
      !alc_context)
  {
    throw std::runtime_error("Could not create audio context");
  }

  // Make context current
  if (!ALC_CALL(alcMakeContextCurrent,
                context_made_current,
                alc_device,
                alc_context) ||
      context_made_current != ALC_TRUE)
  {
    throw std::runtime_error("Could not make audio context current");
  }
}

AudioMaster::~AudioMaster()
{
  Log().d(LOG_TAG) << "Shutdown audio master";

  if (!ALC_CALL(alcMakeContextCurrent,
                context_made_current,
                alc_device,
                nullptr))
  {
    /* what can you do? */
  }

  if (!ALC_CALL(alcDestroyContext, alc_device, alc_context))
  {
    /* not much you can do */
  }

  ALCboolean closed;
  if (!ALC_CALL(alcCloseDevice, closed, alc_device, alc_device))
  {
    /* do we care? */
  }
}
