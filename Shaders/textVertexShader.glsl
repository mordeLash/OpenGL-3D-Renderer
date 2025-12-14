#version 460
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec2 vTextCoord;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBiTangent;

uniform mat4 modelView;
uniform mat4 projection;
uniform mat4 translation;
uniform mat4 rotation;


out vec3 fPos;     // View-space position
out vec3 fNormal;  // Transformed normal in view space
out vec2 fTextCoord;
out mat3 fTBN;

void main() {
    // Transform position to view space
    mat4 transformation =rotation * translation * modelView;
    vec4 viewPos = vPosition * transformation;
    
    fPos = viewPos.xyz;
    
    vec3 T = normalize(vec3(vec4(vTangent,   0.0)*transformation));
    vec3 B = normalize(vec3(vec4(vBiTangent, 0.0)*transformation));
    fNormal = (vNormal * transformation).xyz;


    fTBN = transpose(mat3(T, B, fNormal));

    // Apply projection
    gl_Position = viewPos * projection;
    fTextCoord = vTextCoord;
}