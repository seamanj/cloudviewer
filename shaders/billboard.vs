#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location=0) in vec4 position;


void main() {
  mat4 revisedViewModel = viewMatrix * modelMatrix;
  
  revisedViewModel[0][0] = 1;
  revisedViewModel[0][1] = 0;
  revisedViewModel[0][2] = 0;

  revisedViewModel[1][0] = 0;
  revisedViewModel[1][1] = 1;
  revisedViewModel[1][2] = 0;

  revisedViewModel[2][0] = 0;
  revisedViewModel[2][1] = 0;
  revisedViewModel[2][2] = 1;

  gl_Position = projectionMatrix * revisedViewModel * position;
}
