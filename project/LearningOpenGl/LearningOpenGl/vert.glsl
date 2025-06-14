#version 460 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;


out vec4 vertexColor;
out vec2 TexCoord;

uniform float iTime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position=projection*view*model*vec4(aPos,1.0);//*vec4(aPos.x,aPos.y+sin(aPos.x+iTime)*0.25,aPos.z,1.0);//vec4(aPos,1.0);
	TexCoord=aTexCoord;
}