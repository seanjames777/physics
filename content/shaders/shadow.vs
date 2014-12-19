#version 410 core

uniform mat4 lightWorldViewProjection;
uniform mat4 worldInverseTranspose;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec4 var_position;

void main() {
    vec4 normal = normalize(worldInverseTranspose * vec4(normal, 0));
    vec4 position = lightWorldViewProjection * vec4(position, 1);

    position -= normal * 1.0 / 1000.0;

    gl_Position = position;
    var_position = position;
}
