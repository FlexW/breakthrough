/**
 * Code stolen from:
 * https://indiegamedev.net/2020/02/15/the-complete-guide-to-openal-with-c-part-1-playing-a-sound/
 */

#include <bit>
#include <cstring>
#include <fstream>

#include "log.hpp"
#include "wav-loader.hpp"

static const std::string LOG_TAG = "WavLoader";

static std::int32_t convert_to_int(char *buffer, std::size_t len)
{
  std::int32_t a = 0;
  if (std::endian::native == std::endian::little)
    std::memcpy(&a, buffer, len);
  else
    for (std::size_t i = 0; i < len; ++i)
      reinterpret_cast<char *>(&a)[3 - i] = buffer[i];
  return a;
}

static bool load_wav_file_header(std::ifstream &file,
                                 std::uint8_t & channels,
                                 std::int32_t & sampleRate,
                                 std::uint8_t & bitsPerSample,
                                 std::size_t &  size)
{
  char buffer[4];
  if (!file.is_open())
    return false;

  // the RIFF
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read RIFF";
    return false;
  }
  if (std::strncmp(buffer, "RIFF", 4) != 0)
  {
    Log().e(LOG_TAG) << "file is not a valid WAVE file (header doesn't begin "
                        "with RIFF)";
    return false;
  }

  // the size of the file
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read size of file";
    return false;
  }

  // the WAVE
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read WAVE";
    return false;
  }
  if (std::strncmp(buffer, "WAVE", 4) != 0)
  {
    Log().e(LOG_TAG)
        << "file is not a valid WAVE file (header doesn't contain WAVE)";
    return false;
  }

  // "fmt/0"
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read fmt/0";
    return false;
  }

  // this is always 16, the size of the fmt data chunk
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read the 16";
    return false;
  }

  // PCM should be 1?
  if (!file.read(buffer, 2))
  {
    Log().e(LOG_TAG) << "could not read PCM";
    return false;
  }

  // the number of channels
  if (!file.read(buffer, 2))
  {
    Log().e(LOG_TAG) << "could not read number of channels";
    return false;
  }
  channels = convert_to_int(buffer, 2);

  // sample rate
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read sample rate";
    return false;
  }
  sampleRate = convert_to_int(buffer, 4);

  // (sampleRate * bitsPerSample * channels) / 8
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG)
        << "could not read (sampleRate * bitsPerSample * channels) / 8";
    return false;
  }

  // ?? dafaq
  if (!file.read(buffer, 2))
  {
    Log().e(LOG_TAG) << "could not read dafaq";
    return false;
  }

  // bitsPerSample
  if (!file.read(buffer, 2))
  {
    Log().e(LOG_TAG) << "could not read bits per sample";
    return false;
  }
  bitsPerSample = convert_to_int(buffer, 2);

  // data chunk header "data"
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read data chunk header";
    return false;
  }
  if (std::strncmp(buffer, "data", 4) != 0)
  {
    Log().e(LOG_TAG)
        << "file is not a valid WAVE file (doesn't have 'data' tag)";
    return false;
  }

  // size of data
  if (!file.read(buffer, 4))
  {
    Log().e(LOG_TAG) << "could not read data size";
    return false;
  }
  size = convert_to_int(buffer, 4);

  /* cannot be at the end of file */
  if (file.eof())
  {
    Log().e(LOG_TAG) << "reached EOF on the file";
    return false;
  }
  if (file.fail())
  {
    Log().e(LOG_TAG) << "fail state set on the file";
    return false;
  }

  return true;
}

WavFile load_wav(const std::string &filename)
{
  std::ifstream in(filename, std::ios::binary);

  if (!in.is_open())
  {
    throw std::runtime_error("Could not open \"" + filename + "\"");
  }

  std::size_t  size;
  std::uint8_t channels;
  std::int32_t sample_rate;
  std::uint8_t bits_per_sample;
  if (!load_wav_file_header(in, channels, sample_rate, bits_per_sample, size))
  {
    std::runtime_error("Could not load wav header of \"" + filename + "\"");
  }

  auto data =
      std::vector<unsigned char>(std::istreambuf_iterator<char>(in), {});

  return WavFile(filename, channels, sample_rate, bits_per_sample, size, data);
}
