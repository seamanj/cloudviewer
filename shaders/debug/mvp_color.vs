#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location=0) in vec4 position;


void main() {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
