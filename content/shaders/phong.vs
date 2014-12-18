#version 410 core

uniform mat4 worldInverseTranspose;
uniform mat4 worldViewProjection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 var_normal;
out vec2 var_uv;

void main() {
    gl_Position = worldViewProjection * vec4(position, 1);
    var_normal = normalize((worldInverseTranspose * vec4(normal, 0)).xyz);
    var_uv = uv;
}
