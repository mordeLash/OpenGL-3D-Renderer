#version 460 core
layout(location = 0) in vec3 vPosition;

uniform mat4 projection;

void main() {
    vec4 viewPos = vec4(vPosition, 1.0);
    gl_Position = viewPos * projection;
}