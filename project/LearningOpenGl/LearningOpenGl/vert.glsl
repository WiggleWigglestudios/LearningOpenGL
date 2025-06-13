#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;


out vec4 vertexColor;

uniform float iTime;

void main()
{
	gl_Position=vec4(aPos.x,aPos.y+sin(aPos.x+iTime),aPos.z,1.0);
	vertexColor=vec4(aColor.xyz,1.0);
}