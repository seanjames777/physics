#version 410 core

in vec4 var_position;

out vec4 out_color;

void main() {
    out_color = vec4(vec3(var_position.z / var_position.w), 1);
}
