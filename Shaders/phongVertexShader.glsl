#version 460
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec2 vTextCoord;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBiTangent;

uniform mat4 modelView;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 projection;

out vec3 fPos;     // View-space position
out vec3 fNormal;  // Transformed normal in view space

void main() {
    // Transform position to view space
    vec4 viewPos = vPosition *rotation * translation * modelView;
    fPos = viewPos.xyz;
    
    fNormal = (vNormal * rotation * translation *modelView).xyz;
    
    // Apply projection
    gl_Position = viewPos * projection;
}