#version 330 core

const int corners = 15;

layout(points) in;
layout(triangle_strip, max_vertices = 32) out;

out float depth;

uniform float radius = 0.01;
uniform float aspect_ratio = 1024 / 768.0;

const float PI = 3.1415926;


void diamond(float radius) {
  for (int i = 0; i <= corners; i++) {
    float theta = (PI * 2.0 / corners) * i;

    vec4 offset = vec4(cos(theta), aspect_ratio * sin(theta), 0.0, 0.0);
    gl_Position = gl_in[0].gl_Position + (radius * offset);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
  }
}


void main() {
  depth = gl_in[0].gl_Position.z;

  diamond(radius);
  EndPrimitive();
}
