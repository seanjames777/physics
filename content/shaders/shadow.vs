#version 410 core

layout(std140) uniform SceneUniforms {
    mat4 viewProjection;
    mat4 lightViewProjection;
    vec3 lightDirection;
    int  depthTextureSize;
};

layout(std140) uniform MeshUniforms {
    mat4 world;
    mat4 worldInverseTranspose;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 uv;

out vec4 var_position;

void main() {
    vec4 position = (lightViewProjection * world) * vec4(position, 1);

    gl_Position = position;
    var_position = position;
}
