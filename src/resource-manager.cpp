#include <fstream>

#include <stb/stb_image.h>

#include "resource-manager.hpp"
#include "wav-loader.hpp"

static const std::string LOG_TAG = "ResourceManager";

std::unordered_map<std::string, std::shared_ptr<Texture2D>>
    ResourceManager::textures;

std::unordered_map<std::string, std::shared_ptr<Shader>>
    ResourceManager::shaders;

std::unordered_map<std::string, std::shared_ptr<AudioBuffer>>
    ResourceManager::audio_buffers;

const std::string ResourceManager::resources_directory = "resources";

std::shared_ptr<Shader>
ResourceManager::load_shader(const std::string &vertex_shader_file,
                             const std::string &fragment_shader_file,
                             const std::string &geometry_shader_file,
                             const std::string &name)
{
  shaders[name] = load_shader_from_file(vertex_shader_file,
                                        fragment_shader_file,
                                        geometry_shader_file);
  return shaders[name];
}

std::shared_ptr<Shader>
ResourceManager::load_shader(const std::string &vertex_shader_file,
                             const std::string &fragment_shader_file,
                             const std::string &name)
{
  shaders[name] =
      load_shader_from_file(vertex_shader_file, fragment_shader_file);
  return shaders[name];
}

std::shared_ptr<Shader> ResourceManager::get_shader(const std::string &name)
{
  if (shaders.find(name) == shaders.end())
  {
    throw std::runtime_error("Shader " + name + " not found");
  }

  return shaders[name];
}

std::shared_ptr<Texture2D>
ResourceManager::load_texture(const std::string &file,
                              const bool         alpha,
                              const std::string &name)
{
  textures[name] = load_texture_from_file(file, alpha);
  return textures[name];
}

std::shared_ptr<Texture2D> ResourceManager::get_texture(const std::string &name)
{
  if (textures.find(name) == textures.end())
  {
    throw std::runtime_error("Texture " + name + " not found");
  }

  return textures[name];
}

void ResourceManager::clear()
{
  textures.clear();
  shaders.clear();
  audio_buffers.clear();
}

std::shared_ptr<Shader>
ResourceManager::load_shader_from_file(const std::string &vertex_shader_file,
                                       const std::string &fragment_shader_file,
                                       const std::string &geometry_shader_file)
{
  std::string vertex_code;
  std::string fragment_code;
  std::string geometry_code;

  std::ifstream vertex_shader_file_stream(
      (resources_directory + "/" + vertex_shader_file));

  Log().i(LOG_TAG) << "Load vertex shader from file: "
                   << (resources_directory + "/" + vertex_shader_file);

  std::ifstream fragment_shader_file_stream(
      (resources_directory + "/" + fragment_shader_file));

  Log().i(LOG_TAG) << "Load fragment shader from file: "
                   << (resources_directory + "/" + fragment_shader_file);

  std::stringstream vertex_shader_stream, fragment_shader_stream;
  // read file's buffer contents into streams
  vertex_shader_stream << vertex_shader_file_stream.rdbuf();
  fragment_shader_stream << fragment_shader_file_stream.rdbuf();
  // close file handlers
  vertex_shader_file_stream.close();
  fragment_shader_file_stream.close();
  // convert stream into string
  vertex_code   = vertex_shader_stream.str();
  fragment_code = fragment_shader_stream.str();
  // if geometry shader path is present, also load a geometry shader
  if (geometry_shader_file != "")
  {
    std::ifstream geometry_shader_file_stream(
        (resources_directory + "/" + geometry_shader_file));

    Log().i(LOG_TAG) << "Load geometry shader from file: "
                     << (resources_directory + "/" + geometry_shader_file);

    std::stringstream geometry_shader_stream;
    geometry_shader_stream << geometry_shader_file_stream.rdbuf();
    geometry_shader_file_stream.close();
    geometry_code = geometry_shader_stream.str();
  }

  return std::make_shared<Shader>(vertex_code, fragment_code, geometry_code);
}

std::shared_ptr<Texture2D>
ResourceManager::load_texture_from_file(const std::string &file,
                                        const bool         alpha)
{
  unsigned internal_format = GL_RGB;
  unsigned image_format    = GL_RGB;

  if (alpha)
  {
    internal_format = GL_RGBA;
    image_format    = GL_RGBA;
  }

  Log().i(LOG_TAG) << "Load texture from file: "
                   << (resources_directory + "/" + file);

  int            width, height, nrChannels;
  unsigned char *data = stbi_load((resources_directory + "/" + file).c_str(),
                                  &width,
                                  &height,
                                  &nrChannels,
                                  0);

  const auto texture = std::make_shared<Texture2D>(width,
                                                   height,
                                                   data,
                                                   internal_format,
                                                   image_format);

  stbi_image_free(data);

  return texture;
}

std::shared_ptr<AudioBuffer>
ResourceManager::load_audio(const std::string &audio_file,
                            const std::string &name)
{
  const auto real_audio_file = (resources_directory + "/" + audio_file);

  Log().i(LOG_TAG) << "Load audio from file: " << real_audio_file;

  if (audio_buffers.find(name) != audio_buffers.end())
  {
    return audio_buffers[name];
  }

  auto wav_file = load_wav(real_audio_file);

  // Determine format
  AudioBuffer::Format format;
  if (wav_file.channels == 1 && wav_file.bits_per_sample == 8)
    format = AudioBuffer::Format::MONO8;
  else if (wav_file.channels == 1 && wav_file.bits_per_sample == 16)
    format = AudioBuffer::Format::MONO16;
  else if (wav_file.channels == 2 && wav_file.bits_per_sample == 8)
    format = AudioBuffer::Format::STEREO8;
  else if (wav_file.channels == 2 && wav_file.bits_per_sample == 16)
    format = AudioBuffer::Format::STEREO16;
  else
  {
    throw std::runtime_error(
        "Unrecognised wave format: " + std::to_string(wav_file.channels) +
        " channels, " + std::to_string(wav_file.bits_per_sample) + " bps");
  }

  auto audio_buffer = std::make_shared<AudioBuffer>(format,
                                                    wav_file.data,
                                                    wav_file.sample_rate,
                                                    wav_file.size);

  audio_buffers[name] = audio_buffer;

  return audio_buffer;
}

std::shared_ptr<AudioBuffer> ResourceManager::get_audio(const std::string &name)
{
  if (audio_buffers.find(name) == audio_buffers.end())
  {
    throw std::runtime_error("Audio buffer " + name + " not found");
  }

  return audio_buffers[name];
}
