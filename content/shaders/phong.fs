#version 410 core

in vec3 var_normal;
in vec2 var_uv;

out vec4 out_color;

void main() {
    vec3 light_dir = normalize(vec3(1, 1, 1));
    vec3 normal = normalize(var_normal);

    float lambert = clamp(dot(var_normal, light_dir), 0, 1);

    lambert = lambert * .8 + .2;

    out_color = vec4(lambert, lambert, lambert, 1);
}
