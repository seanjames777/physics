#version 330 core

in vec2 var_uv;
in vec4 var_color;

out vec4 out_color;

uniform sampler2D atlas;

void main() {
    float f = texture(atlas, var_uv).r;
    out_color = vec4(1, 1, 1, f) * var_color;
}
