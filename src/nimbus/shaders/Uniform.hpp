#pragma once

#include <iostream>
#include <string>

#include "nimbus/glitter.hpp"
#include "nimbus/utils/Color.hpp"


template <typename T>
class Uniform {
public:
  Uniform(const std::string& name, GLint program)
    : _program(program),
    _name(name),
    _location(glGetUniformLocation(program, name.c_str()))
  {
    if (_location < 0) {
      throw std::logic_error("Failed to get uniform location for '" + name + "'");
    }
  }

  bool set(const T& value) {
    glUseProgram(_program);
    glUniform(_location, value);
    auto error = glGetError();
    if (error != GL_NO_ERROR) {
      std::cerr << "Error setting uniform " << _name << " in program " << _program << "!" << std::endl;
    }
    return error == GL_NO_ERROR;
  }

  const T& get() {
    return _value;
  }

private:
  GLint _program;
  std::string _name;
  GLint _location;
  T _value;
};



template <>
inline bool Uniform<float>::set(const float& value) {
  glUseProgram(_program);
  glUniform1f(_location, value);
  return glGetError() == GL_NO_ERROR;
}


template <>
inline bool Uniform<bool>::set(const bool& value) {
  glUseProgram(_program);
  glUniform1i(_location, value ? 1 : 0);
  return glGetError() == GL_NO_ERROR;
}


template <>
inline bool Uniform<Color>::set(const Color& value) {
  glUseProgram(_program);
  glUniform(_location, value.toOpenGl());
  return glGetError() == GL_NO_ERROR;
}
