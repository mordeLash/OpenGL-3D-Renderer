#version 460 //compatible with OpenGL version 4.6 (latest version)
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec2 vTextCoord;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBiTangent;

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 translation;
uniform mat4 rotation;
uniform float maxD;
uniform float animation_time;
uniform float time;
uniform bool full_deform;


//These colors will be interpolated by the rasterizer for each triangle after the vertex shader terminates.
//The interpolated colors will become the input of the fragment shader
out vec3 fPos;     // View-space position
out vec3 fNormal;  // Transformed normal in view space


void main()
{
	//gl_Position is a vec4 built-in GLSL output variable that holds the transformed vertex position
	float t = 0;
	if(animation_time != 0){
		t=mod(time, animation_time) / (animation_time * 0.5);//[0-2]
	}
	float progress; 
	progress = abs(t - 1.0)*maxD;//t goes from 0-2 and the function is symerical around t=1

	if(!full_deform){
		if(mod(time+animation_time*0.5, animation_time*2)>animation_time){
			if(abs(round(vPosition.x)-vPosition.x)>=0.25){
				progress = 0;
			}
		}
		else{
			if(abs(round(vPosition.x)-vPosition.x)<0.25){
			progress = 0;
			}
		}
	}
	vec4 viewPos = (vPosition+(progress*vNormal));
    viewPos = viewPos *rotation * translation * modelView;

    fPos = viewPos.xyz;
    
    fNormal = (vNormal * rotation * translation *modelView).xyz;
    // Apply projection
    gl_Position = viewPos * projection;

	
}
