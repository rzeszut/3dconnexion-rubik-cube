#version 330 core

in vec3 vertexPosition;
in vec2 vertexUV;

out vec2 uv;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1);

    uv = vertexUV;
}
