#version 460 core
out vec4 FragColor;
in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D inputTexture;
void main()
{
	FragColor = texture(inputTexture,TexCoord)*vertexColor;//vec4(1.0f, 0.5f, 0.2f, 1.0f);
}