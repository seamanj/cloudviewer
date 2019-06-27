#version 330

layout (location=0) in vec4 position;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec4 vertexNormal;

out vec2 outTexCoord;
out vec3 mvVertexNormal;
out vec3 mvVertexPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  mat4 modelViewMatrix = viewMatrix * modelMatrix;
  vec4 mvPos = modelViewMatrix * position;
  gl_Position = projectionMatrix * mvPos;
  outTexCoord = texCoord;
  mvVertexNormal = normalize(modelViewMatrix * vertexNormal).xyz;
  mvVertexPos = mvPos.xyz;
}
