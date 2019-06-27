#version 330

in vec2 outTexCoord;
uniform sampler2D texture_sampler;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

out vec4 fragColor;

void main() {
    fragColor = texture(texture_sampler, outTexCoord) * color;
}
