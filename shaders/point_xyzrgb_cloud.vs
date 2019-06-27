#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout (location=0) in vec4 position;
layout (location=1) in vec4 color;

out vec4 vertexColor;


void main() {
  gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
  vertexColor = color / 255.0;
}
