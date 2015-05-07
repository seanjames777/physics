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

out vec4 var_lightPosition;
out vec3 var_normal;
out vec2 var_uv;

void main() {
    var_lightPosition = (lightViewProjection * world) * vec4(position, 1);
    gl_Position = (viewProjection * world) * vec4(position, 1);
    var_normal = (worldInverseTranspose * vec4(normal, 0)).xyz;
    var_uv = uv;
}
