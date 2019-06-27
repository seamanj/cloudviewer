#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location=0) in vec4 position;


void main() {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
  if (gl_Position.x == 0 && gl_Position.y == 0 && gl_Position.z == 0) {
      gl_Position.x = 100.0;
  }
}
