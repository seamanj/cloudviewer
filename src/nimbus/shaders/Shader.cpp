#include "nimbus/shaders/Shader.hpp"
#include "Shader.hpp"


#include <iostream>


#include <cmrc/cmrc.hpp>
CMRC_DECLARE(shaders);


nonstd::optional<GLuint> Shader::compile(const std::string& src, GLenum type) {
  const char* source = src.c_str();
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint status = false;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::unique_ptr<char[]> buffer(new char[length]);
    glGetShaderInfoLog(shader, length, nullptr, buffer.get());
    fprintf(stderr, "%s", buffer.get());
    glDeleteShader(shader);
    return {};
  }

  return {shader};
}


nonstd::optional<GLuint> Shader::compile_resource(const std::string& resource, GLenum type) {
  auto filesystem = cmrc::shaders::get_filesystem();
  if (!filesystem.is_file(resource)) {
    throw std::logic_error(resource + " does not exist!");
  }
  auto src = filesystem.open(resource);
  return compile(std::string(src.begin(), src.size()), type);
}


template <typename ShaderClass>
nonstd::optional<ShaderClass> compile(const std::string& source) {
  auto compiled = Shader::compile(source, ShaderClass::TYPE);

  if (!compiled) {
    return {};
  }

  return {ShaderClass(*compiled)};
}


template <typename ShaderClass>
nonstd::optional<ShaderClass> compile_resource(const std::string& resource) {
  auto compiled = Shader::compile_resource(resource, ShaderClass::TYPE);

  if (!compiled) {
    return {};
  }

  return {ShaderClass(*compiled)};
}


nonstd::optional<FragmentShader> FragmentShader::compile_resource(const std::string& resource) {
  return ::compile_resource<FragmentShader>(resource);
}


nonstd::optional<GeometryShader> GeometryShader::compile_resource(const std::string& resource) {
  return ::compile_resource<GeometryShader>(resource);
}


nonstd::optional<VertexShader> VertexShader::compile_resource(const std::string& resource) {
  return ::compile_resource<VertexShader>(resource);
}


std::shared_ptr<ShaderProgram> ShaderProgram::link(const VertexShader& vertex, const FragmentShader& fragment) {
  return link({&vertex, &fragment});
}


std::shared_ptr<ShaderProgram> ShaderProgram::link(const VertexShader& vertex, 
                                                   const GeometryShader& geometry,
                                                   const FragmentShader& fragment) {
  return link({&vertex, &geometry, &fragment});
}


std::shared_ptr<ShaderProgram> ShaderProgram::link(std::vector<const Shader*> shaders) {
  auto program = glCreateProgram();

  for (auto shader : shaders) {
    glAttachShader(program, *shader);
  }

  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if(!status) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::unique_ptr<char[]> buffer(new char[length]);
    glGetProgramInfoLog(program, length, nullptr, buffer.get());
    fprintf(stderr, "%s", buffer.get());
    glDeleteProgram(program);
    return {};
  }

  return std::make_shared<ShaderProgram>(program);
}
