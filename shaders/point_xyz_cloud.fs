#version 330

uniform vec4 color;
out vec4 fragColor;

uniform float depth_threshold = 100.0; // linear drop from depth = 0 to this value.
in float depth;

void main()
{
    float opacity = (-1 / (depth_threshold * depth_threshold)) * (depth * depth) + 1;
    fragColor = color;
    fragColor.a = opacity;
}
