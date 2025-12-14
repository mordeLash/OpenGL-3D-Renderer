#version 460 //compatible with OpenGL version 4.6 (latest version)

//Interpolated colors coming from the rasterizer based on the per vertex color that is output by the vertex shader
in vec4 color;
//The final color of the fragement. If depth testing is passed, this color will occupy the corresponding pixel
out vec4 outFragColor;

void main()
{
	outFragColor = color; //pass through
}
