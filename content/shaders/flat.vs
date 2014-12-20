#version 410 core

uniform mat4 viewProjection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 color;

out vec3 var_color;

void main() {
    gl_Position = viewProjection * vec4(position, 1);
    var_color = color;
}
