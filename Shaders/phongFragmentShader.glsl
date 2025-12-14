#version 460
struct Light { vec3 pos_dir; vec3 intensity; int type; };
in vec3 fPos;
in vec3 fNormal;


uniform vec3 camPos;
uniform vec4 objColor;
uniform vec4 objCoeffs; // x:ambient, y:diffuse, z:specular, w:shininess
uniform Light lights[3];
uniform bool double_sided;


out vec4 fragColor;

void main()
{

	vec4 color = vec4(0,0,0,0);
	vec3 temp_pos = fPos;
	vec3 N = normalize(fNormal);
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
		else if(lights[i].type==1){
			L = lights[i].pos_dir;
			L *= -1;
		}
		else if(lights[i].type==2){
			L = normalize(lights[i].pos_dir - temp_pos);
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

}