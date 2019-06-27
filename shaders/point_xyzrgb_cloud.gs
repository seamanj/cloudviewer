#version 330 core

const int corners = 4;

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

uniform float radius = 0.01;

uniform float aspect_ratio = 1280 / 800.0;

const float PI = 3.1415926;

in vec4 vertexColor[];
out vec4 geomColor;


void diamond(float radius) {
  for (int i = 0; i <= corners; i++) {
    float theta = PI * 2.0 / corners * i;

    vec4 offset = vec4(cos(theta), aspect_ratio * -sin(theta), 0.0, 0.0);
    gl_Position = gl_in[0].gl_Position + (radius * offset);

    EmitVertex();
  }
}


void main() {
  geomColor = vertexColor[0];
  diamond(radius);
  EndPrimitive();
}
