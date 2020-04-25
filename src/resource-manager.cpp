#include <fstream>

#include <stb/stb_image.h>

#include "resource-manager.hpp"

std::unordered_map<std::string, std::shared_ptr<Texture2D>>
    ResourceManager::textures;
std::unordered_map<std::string, std::shared_ptr<Shader>>
    ResourceManager::shaders;

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

std::shared_ptr<Shader> ResourceManager::get_shader(const std::string &name)
{
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
  return textures[name];
}

void ResourceManager::clear()
{
  textures.clear();
  shaders.clear();
}

std::shared_ptr<Shader>
ResourceManager::load_shader_from_file(const std::string &vertex_shader_file,
                                       const std::string &fragment_shader_file,
                                       const std::string &geometry_shader_file)
{
  std::string vertex_code;
  std::string fragment_code;
  std::string geometry_code;

  std::ifstream vertex_shader_file_stream(vertex_shader_file);
  std::ifstream fragment_shader_file_stream(fragment_shader_file);

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
  if (!geometry_shader_file.empty())
  {
    std::ifstream     geometry_shader_file_stream(geometry_shader_file);
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

  int            width, height, nrChannels;
  unsigned char *data =
      stbi_load(file.c_str(), &width, &height, &nrChannels, 0);

  const auto texture = std::make_shared<Texture2D>(width,
                                                   height,
                                                   data,
                                                   internal_format,
                                                   image_format);

  stbi_image_free(data);

  return texture;
}
