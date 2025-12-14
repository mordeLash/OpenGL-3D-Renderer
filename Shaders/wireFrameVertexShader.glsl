#version 460
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
//layout(location = 2) in vec2 vTextCoord;

uniform mat4 modelView;
uniform mat4 projection;
uniform mat4 translation;
uniform mat4 rotation;


void main() {
    // Transform position to view space
    vec4 viewPos = vPosition * rotation * translation * modelView;
    gl_Position = viewPos * projection;
}