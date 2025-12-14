#version 460
struct Light { vec3 pos_dir; vec3 intensity; int type; };
in vec3 fPos;
in vec3 fNormal;
in vec2 fTextCoord;
in mat3 fTBN;


uniform sampler2D texMap;
uniform sampler2D normalMap;

uniform vec3 camPos;
uniform vec4 objColor;
uniform vec4 objCoeffs; // x:ambient, y:diffuse, z:specular, w:shininess
uniform Light lights[3];
uniform bool map;
uniform bool texturise;
uniform bool double_sided;
uniform bool has_texture;
uniform bool has_normal;


out vec4 fragColor;

void main()
{
	vec4 color = vec4(0,0,0,0);
	vec3 temp_pos = fPos;
	vec3 N = normalize(fNormal);

	if(map){
		if(has_normal){
			N = texture(normalMap, fTextCoord).rgb;
			N = N * 2.0 - 1.0;   
			N = normalize(fTBN*N); 
		}
		else{
			fragColor = vec4(normalize(fTBN[2]) * 0.5 + 0.5, 1.0); // Visualize normal (Blue)
			return;
		}
	}
	
	vec3 V = normalize(temp_pos - camPos);
	vec3 L = vec3(0,0,0);

	if (double_sided && N.z < 0){
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
	fragColor[0] = clamp(color[0]*objColor[3],0,1);
	fragColor[1] = clamp(color[1]*objColor[2],0,1);
	fragColor[2] = clamp(color[2]*objColor[1],0,1);
	
	if(texturise){
		if(has_texture){
			fragColor = texture(texMap, fTextCoord)*fragColor;
		}
		else
		{
			fragColor = vec4(normalize(fTextCoord) * 0.5 + 0.5,0.0, 1.0);
		}
		
	}
}