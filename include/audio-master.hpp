#pragma once

#include "open-al.hpp"

class AudioMaster
{
public:
  AudioMaster();

  ~AudioMaster();

private:
  ALCdevice * alc_device           = nullptr;
  ALCcontext *alc_context          = nullptr;
  ALCboolean  context_made_current = ALC_FALSE;
};
