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

in vec4 var_position;

out vec4 out_color;

void main() {
    out_color = vec4(vec3(var_position.z / var_position.w), 1);
}
