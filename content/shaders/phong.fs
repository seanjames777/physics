#version 410 core

uniform sampler2D depthTexture;
uniform sampler2D diffuseTexture;
uniform vec3 lightDirection;
uniform int depthTextureSize;

in vec4 var_lightPosition;
in vec3 var_normal;
in vec2 var_uv;

out vec4 out_color;

void main() {
    // Clamp allows us to use CLAMP_TO_EDGE as a hack for regions outside shadow map
    float actual_depth = clamp(var_lightPosition.z / var_lightPosition.w, -1.0, 1.0);
    actual_depth -= 1.0 / 100.0;

    vec2 shadow_uv = var_lightPosition.xy / var_lightPosition.w;
    shadow_uv = shadow_uv * 0.5 + 0.5;

    float shadow = 0.0;
    float samples = 0.0;

    // Lousy PCF
    // TODO
    vec2 offset = vec2(1.0 / float(depthTextureSize));

    for (float y = -1.5; y <= 1.5; y += 1.0)
        for (float x = -1.5; x <= 1.5; x += 1.0) {
            vec2 sample_pos = shadow_uv + offset * vec2(x, y);
            float shadow_depth = texture(depthTexture, sample_pos).r;
            shadow += actual_depth > shadow_depth ? 0.0f : 1.0f;
            samples += 1.0;
        }

    shadow /= samples;
    shadow = shadow * .8 + .2;

    vec3 normal = normalize(var_normal);

    float lambert = clamp(dot(var_normal, lightDirection), 0, 1);

    lambert = lambert * .8 + .2;
    lambert *= shadow;

    vec4 diffuse = texture(diffuseTexture, var_uv);

    out_color = vec4(diffuse.rgb * lambert, 1);
}
