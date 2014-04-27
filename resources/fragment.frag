#version 330 core

in vec2 uv;

out vec3 color;

uniform sampler2D textureSampler;

void main() {
    vec2 inverted_uv = vec2(uv.x, 1 - uv.y);
    color = texture(textureSampler, inverted_uv).rgb;
}
