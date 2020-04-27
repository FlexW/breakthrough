#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "asseration.hpp"
#include "log.hpp"

/**
 * @brief General purpose Shader class
 *
 * Handles creation and linking of shaders.
 */
class Shader
{
public:
  /**
   * Creates a new Shader
   *
   * @param vertexShaderProgram Path to vertex shader
   * @param fragmentShaderProgram Path to fragment shader
   * @param geometryShaderProgram Path to geometry shader. This is optional
   */
  Shader(const std::string &vertexShaderProgram,
         const std::string &fragmentShaderProgram,
         const std::string &geometryShaderProgram = "");

  /**
   * Get the id of the shader
   *
   * @return Shader id
   */
  unsigned int get_id() const { return id; }

  /**
   * Binds the shader for rendering
   */
  void bind() const { glUseProgram(id); }

  /**
   * Unbinds the shader for rendering
   */
  void unbind() const { glUseProgram(0); }

  /**
   * Sets a bool variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, bool value)
  {
    glUniform1i(get_uniform_location(name), (int)value);
  }
  // void set_bool(const std::string &name, bool value)
  // {
  //   glUniform1i(getUniformLocation(name), (int)value);
  // }

  /**
   * Sets a int variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, int value)
  {
    glUniform1i(get_uniform_location(name), value);
  }

  template <std::size_t SIZE>
  void set_uniform(const std::string &name, std::array<float, SIZE> &values)
  {
    glUniform1fv(get_uniform_location(name),
                 values.size(),
                 reinterpret_cast<GLfloat *>(&values[0]));
  }

  template <std::size_t SIZE>
  void set_uniform(const std::string &name, std::array<int, SIZE> &values)
  {
    glUniform1iv(get_uniform_location(name),
                 values.size(),
                 reinterpret_cast<GLint *>(&values[0]));
  }

  // FIXME: Handle ints?
  template <std::size_t SIZE>
  void set_uniform(const std::string &name, std::array<glm::vec2, SIZE> &values)
  {
    glUniform1fv(get_uniform_location(name),
                 values.size(),
                 reinterpret_cast<GLfloat *>(&values[0]));
  }

  /**
   * Sets a float variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, float value)
  {
    glUniform1f(get_uniform_location(name), static_cast<GLfloat>(value));
  }

  /**
   * Sets a vec2 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::vec2 &value)
  {
    glUniform2fv(get_uniform_location(name), 1, &value[0]);
  }

  /**
   * Sets a vec3 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::vec3 &value)
  {
    glUniform3fv(get_uniform_location(name), 1, &value[0]);
  }

  /**
   * Sets a vec4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::vec4 &value)
  {
    glUniform4fv(get_uniform_location(name), 1, &value[0]);
  }

  /**
   * Sets a mat2 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::mat2 &value)
  {
    glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, &value[0][0]);
  }

  /**
   * Sets a mat3 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::mat3 &value)
  {
    glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, &value[0][0]);
  }

  /**
   * Sets a mat4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void set_uniform(const std::string &name, const glm::mat4 &value)
  {
    glUniformMatrix4fv(get_uniform_location(name),
                       1,
                       GL_FALSE,
                       glm::value_ptr(value));
  }

  /**
   * Sets a mat4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  template <std::size_t SIZE>
  void set_uniform(const std::string &name, std::array<glm::mat4, SIZE> &value)
  {
    glUniformMatrix4fv(get_uniform_location(name),
                       value.size(),
                       GL_FALSE,
                       glm::value_ptr(value));
  }

  /**
   * Get the location of a attribute
   *
   * @param name Name of the attribute
   *
   * @return Attribute location
   */
  GLuint get_attrib_location(const std::string &name) const
  {
    return glGetAttribLocation(id, name.c_str());
  }

  int get_uniform_location(const std::string &name)
  {
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
      return uniformLocationCache[name];
    }
    auto location = glad_glGetUniformLocation(id, name.c_str());

    if (location == -1)
    {
      Log().w(LOG_TAG) << "Uniform " << name << " doesn't exist!";
    }

    uniformLocationCache[name] = location;
    return location;
  }

private:
  static const std::string LOG_TAG;

  unsigned int                         id = 0;
  std::unordered_map<std::string, int> uniformLocationCache;

  enum class ShaderType
  {
    PROGRAMM,
    FRAGMENT,
    VERTEX,
    GEOMETRY
  };

  std::string shader_type_to_string(ShaderType shaderType) const;

  void check_for_compile_errors(const GLuint     shaderId,
                                const ShaderType shaderType);
};
