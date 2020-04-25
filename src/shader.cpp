#include "shader.hpp"
#include "asseration.hpp"

const std::string Shader::LOG_TAG = "Shader";

Shader::Shader(const std::string &vertexShaderProgram,
               const std::string &fragmentShaderProgram,
               const std::string &geometryShaderProgram)
{
  unsigned int vertexShaderId = 0, fragmentShaderId = 0, geometryShaderId = 0;
  vertexShaderId            = glCreateShader(GL_VERTEX_SHADER);
  auto vertexShaderCodeCStr = vertexShaderProgram.c_str();
  glShaderSource(vertexShaderId, 1, &vertexShaderCodeCStr, NULL);
  glCompileShader(vertexShaderId);
  checkForCompileErrors(vertexShaderId, ShaderType::VERTEX);

  fragmentShaderId            = glCreateShader(GL_FRAGMENT_SHADER);
  auto fragmentShaderCodeCStr = fragmentShaderProgram.c_str();
  glShaderSource(fragmentShaderId, 1, &fragmentShaderCodeCStr, NULL);
  glCompileShader(fragmentShaderId);
  checkForCompileErrors(fragmentShaderId, ShaderType::FRAGMENT);

  id = glCreateProgram();
  glAttachShader(id, vertexShaderId);
  glAttachShader(id, fragmentShaderId);

  if (!geometryShaderProgram.empty())
  {
    geometryShaderId            = glCreateShader(GL_GEOMETRY_SHADER);
    auto geometryShaderCodeCStr = geometryShaderProgram.c_str();
    glShaderSource(geometryShaderId, 1, &geometryShaderCodeCStr, NULL);
    glCompileShader(geometryShaderId);
    checkForCompileErrors(geometryShaderId, ShaderType::GEOMETRY);
    glAttachShader(id, geometryShaderId);
  }

  glLinkProgram(id);
  checkForCompileErrors(id, ShaderType::PROGRAMM);

  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);
}

void Shader::checkForCompileErrors(const GLuint     shaderId,
                                   const ShaderType shaderType)
{
  GLint     success;
  const int infoLogSize = 1024;
  GLchar    infoLog[infoLogSize];

  switch (shaderType)
  {
  case ShaderType::VERTEX:
  case ShaderType::FRAGMENT:
  case ShaderType::GEOMETRY:
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shaderId, infoLogSize, NULL, infoLog);

      std::string message =
          shaderTypeToString(shaderType) + " Shader Error:\n" + infoLog;
      throw std::runtime_error(message);
    }
    return;

  case ShaderType::PROGRAMM:
    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shaderId, infoLogSize, NULL, infoLog);

      std::string message =
          shaderTypeToString(shaderType) + " Shader Error:\n" + infoLog;
      Log().e(LOG_TAG) << message;
    }
    break;

  default:
    FAIL;
  }
}

std::string Shader::shaderTypeToString(ShaderType shaderType) const
{
  switch (shaderType)
  {
  case ShaderType::VERTEX:
    return "Vertex";
  case ShaderType::FRAGMENT:
    return "Fragment";
  case ShaderType::PROGRAMM:
    return "Programm";
  case ShaderType::GEOMETRY:
    return "Geometry";
  default:
    FAIL;
  }
}
