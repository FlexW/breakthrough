#pragma once

#include <array>
#include <vector>

#include "open-al.hpp"

class WavFile
{
public:
  const std::string                name;
  const std::uint8_t               channels;
  const std::int32_t               sample_rate;
  const std::uint8_t               bits_per_sample;
  const std::size_t                size;
  const std::vector<unsigned char> data;

  WavFile(const std::string &               name,
          const std::uint8_t                channels,
          const std::int32_t                sample_rate,
          const std::uint8_t                bits_per_sample,
          const std::size_t                 size,
          const std::vector<unsigned char> &data)
      : name(name),
        channels(channels),
        sample_rate(sample_rate),
        bits_per_sample(bits_per_sample),
        size(size),
        data(data)
  {
  }
};

WavFile load_wav(const std::string &filename);
