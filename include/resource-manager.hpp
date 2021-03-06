#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <glad/glad.h>

#include "audio-buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"

class ResourceManager
{
public:
  static std::shared_ptr<Shader>
  load_shader(const std::string &vertex_shader_file,
              const std::string &fragment_shader_file,
              const std::string &geometry_shader_file,
              const std::string &name);

  static std::shared_ptr<Shader>
  load_shader(const std::string &vertex_shader_file,
              const std::string &fragment_shader_file,
              const std::string &name);

  static std::shared_ptr<Shader> get_shader(const std::string &name);

  static std::shared_ptr<Texture2D> load_texture(const std::string &file,
                                                 const bool         alpha,
                                                 const std::string &name);

  static std::shared_ptr<Texture2D> get_texture(const std::string &name);

  static std::shared_ptr<AudioBuffer> load_audio(const std::string &audio_file,
                                                 const std::string &name);

  static std::shared_ptr<AudioBuffer> get_audio(const std::string &name);

  static void clear();

private:
  static const std::string resources_directory;

  static std::unordered_map<std::string, std::shared_ptr<Shader>>    shaders;
  static std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
  static std::unordered_map<std::string, std::shared_ptr<AudioBuffer>>
      audio_buffers;

  ResourceManager()                   = delete;
  ~ResourceManager()                  = delete;
  ResourceManager(ResourceManager &)  = delete;
  ResourceManager(ResourceManager &&) = delete;

  static std::shared_ptr<Shader>
  load_shader_from_file(const std::string &vertex_shader_file,
                        const std::string &fragment_shader_file,
                        const std::string &geometry_shader_file = "");

  static std::shared_ptr<Texture2D>
  load_texture_from_file(const std::string &file, const bool alpha);
};
