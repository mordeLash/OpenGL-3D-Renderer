#version 460 //compatible with OpenGL version 4.6 (latest version)
struct Light {
    vec3 pos_dir;
    vec3 intensity;
    int type;
};
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec2 vTextCoord;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBiTangent;

uniform mat4 modelView; 
uniform mat4 projection;
uniform mat4 translation;
uniform mat4 rotation;

uniform bool double_sided;
uniform vec4 objColor;
uniform vec4 objCoeffs;
uniform vec3 camPos;
uniform Light lights[3];

//These colors will be interpolated by the rasterizer for each triangle after the vertex shader terminates.
//The interpolated colors will become the input of the fragment shader
out vec4 color;


void main()
{
	//gl_Position is a vec4 built-in GLSL output variable that holds the transformed vertex position
	gl_Position = vPosition;

	//uniform scaling - dividing the w coordinate is like multiplying the x, y, z coordinates
	//gl_Position.w = gl_Position.w / scale;
	vec4 normal;
	gl_Position = gl_Position * rotation * translation * modelView;
	normal = normalize(vNormal*rotation * translation * modelView);
	color = vec4(0,0,0,0);
	vec3 temp_pos = vec3(gl_Position.x,gl_Position.y,gl_Position.z);
	vec3 N = vec3(normal.x,normal.y,normal.z);
	gl_Position = gl_Position * projection;
	vec3 V = normalize(temp_pos - camPos);
	vec3 L = vec3(0,0,0);

	if (N.z < 0 && double_sided)
	{
		N = -N;
	}

	for(int i=0; i<3; i++){
		if(lights[i].type==4){
			continue;
		}
		else if(lights[i].type==3){
			color[0] += lights[i].intensity[0] * objCoeffs[0];
			color[1] += lights[i].intensity[1] * objCoeffs[0];
			color[2] += lights[i].intensity[2] * objCoeffs[0];
			continue;
		}
		else if(lights[i].type==2){
			L = normalize(lights[i].pos_dir - temp_pos);
		}
		else if(lights[i].type==1){
			L = lights[i].pos_dir;
			L *= -1;
		}
		vec3 Rn = reflect(L,N);
		if (N.z >= 0)
			{
				float Ids = (objCoeffs[1] * max(dot(N, L),0.0)) + (objCoeffs[2] * pow(max(dot(Rn, V),0.0), objCoeffs[3]));
				color[0] += lights[i].intensity[0] * Ids;
				color[1] += lights[i].intensity[1] * Ids;
				color[2] += lights[i].intensity[2] * Ids;
			}
	}
	color[0] = clamp(color[0]*objColor[3],0,1);
	color[1] = clamp(color[1]*objColor[2],0,1);
	color[2] = clamp(color[2]*objColor[1],0,1);
		

	//You need to add your code for Gourard shading here instead of the pass through of the color above
	//There are many useful built-in functions in GLSL such as:
	//reflect,
	//dot,
	//cross,
	//length,
	//faceforward,
	//min,
	//max,
	//sin,
	//degrees,
	//radians
	//clamp,
	//ceil,
	//fract, etc.
	//Look for the "Built-In Functions" page in the OpenGL Reference Guide
}
