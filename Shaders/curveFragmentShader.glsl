#version 460

out vec4 fragColor;

uniform vec4 vColor;

void main()
{
	fragColor = vColor;
}