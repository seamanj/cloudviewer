#pragma once

#include <memory>
#include <vector>

#include "nimbus/glitter.hpp"

#include "nimbus/utils/optional.hpp"


class OpenGLResource {
public:
  OpenGLResource(GLuint id)
    : _id(id)
  {
  }

  operator GLuint() const {
    return _id;
  }

  GLuint id() const {
    return _id;
  }

  const GLuint _id;
};


class Shader : public OpenGLResource {
public:
  static nonstd::optional<GLuint> compile(const std::string& source, GLenum type);
  static nonstd::optional<GLuint> compile_resource(const std::string& resource, GLenum type);

protected:
  Shader(GLuint id, GLenum type)
    : OpenGLResource(id), _type(type)
  {
  }

  const GLenum _type;
};


class VertexShader : public Shader {
public:
  static nonstd::optional<VertexShader> compile_resource(const std::string& resource);

  static constexpr auto TYPE = GL_VERTEX_SHADER;

  VertexShader(GLuint id)
    : Shader(id, TYPE)
  {
  }
};


class GeometryShader : public Shader {
public:
  static nonstd::optional<GeometryShader> compile_resource(const std::string& resource);

  static constexpr auto TYPE = GL_GEOMETRY_SHADER;

  GeometryShader(GLuint id)
    : Shader(id, TYPE)
  {
  }
};


class FragmentShader : public Shader {
public:
  static nonstd::optional<FragmentShader> compile_resource(const std::string& resource);

  static constexpr auto TYPE = GL_FRAGMENT_SHADER;

  FragmentShader(GLuint id)
    : Shader(id, TYPE)
  {
  }
};


class ShaderProgram : public OpenGLResource {
public:
  static std::shared_ptr<ShaderProgram> link(const VertexShader& vertex, const FragmentShader& fragment);
  static std::shared_ptr<ShaderProgram> link(const VertexShader& vertex, 
                                             const GeometryShader& geometry,
                                             const FragmentShader& fragment);

  ShaderProgram(GLuint id)
    : OpenGLResource(id)
  {
  }

  virtual ~ShaderProgram() = default;

  virtual void before_draw() {};

protected:
  static std::shared_ptr<ShaderProgram> link(std::vector<const Shader*> shaders);
};
