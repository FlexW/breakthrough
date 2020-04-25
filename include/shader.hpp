#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "log.hpp"

class Shader
{
public:
  /**
   * Creates a new Shader
   *
   * @param vertexShaderProgram Path to vertex shader
   * @param fragmentShaderProgram Path to fragment shader
   */
  Shader(const std::string &vertexShaderProgram,
         const std::string &fragmentShaderProgram);

  /**
   * Get the id of the shader
   *
   * @return Shader id
   */
  unsigned int getId() const { return id; }

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
  void setBool(const std::string &name, bool value)
  {
    glUniform1i(getUniformLocation(name), (int)value);
  }

  /**
   * Sets a int variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setInt(const std::string &name, int value)
  {
    glUniform1i(getUniformLocation(name), value);
  }

  /**
   * Sets a float variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
  }

  /**
   * Sets a vec2 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setVec2(const std::string &name, const glm::vec2 &value)
  {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
  }

  /**
   * Sets a vec3 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setVec3(const std::string &name, const glm::vec3 &value)
  {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
  }

  /**
   * Sets a vec4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setVec4(const std::string &name, const glm::vec4 &value)
  {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
  }

  /**
   * Sets a mat2 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setMat2(const std::string &name, const glm::mat2 &value)
  {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
  }

  /**
   * Sets a mat3 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setMat3(const std::string &name, const glm::mat3 &value)
  {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
  }

  /**
   * Sets a mat4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   */
  void setMat4(const std::string &name, const glm::mat4 &value)
  {
    glUniformMatrix4fv(getUniformLocation(name),
                       1,
                       GL_FALSE,
                       glm::value_ptr(value));
  }

  /**
   * Sets a mat4 variable for the shader
   *
   * @param name Name of the variable
   * @param value Value
   * @param count Number of matrices
   */
  void setMultipleMat4(const std::string &name,
                       const glm::mat4 &  value,
                       const unsigned     count)
  {
    glUniformMatrix4fv(getUniformLocation(name),
                       count,
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
  GLuint getAttribLocation(const std::string &name) const
  {
    return glGetAttribLocation(id, name.c_str());
  }

  int getUniformLocation(const std::string &name)
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
    VERTEX
  };

  std::string shaderTypeToString(ShaderType shaderType) const;

  void checkForCompileErrors(const GLuint     shaderId,
                             const ShaderType shaderType);
};
