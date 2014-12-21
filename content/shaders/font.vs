#version 330 core

uniform vec2 screenSz;
uniform vec2 atlasSz;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

out vec2 var_uv;
out vec4 var_color;

void main() {
    // Don't need to align to pixel centers under GL
    vec2 screen_pos = position.xy / screenSz;
    screen_pos.y = 1.0 - screen_pos.y;
    screen_pos = screen_pos * 2.0 - 1.0;

    gl_Position = vec4(screen_pos, 0, 1);

    var_uv = uv / atlasSz;
    var_uv = vec2(var_uv.x, var_uv.y);

    var_color = color;
}
