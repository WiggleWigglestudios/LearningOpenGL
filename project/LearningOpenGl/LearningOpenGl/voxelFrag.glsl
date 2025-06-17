#version 460 core

out vec4 FragColor;
in vec3 TexCoord;
in vec4 gl_FragCoord;

uniform sampler3D volumeTexture;
uniform vec2 windowSize;
uniform vec3 cameraPos;
uniform vec3 cameraLookDir;
uniform vec3 voxelSize;
uniform mat4 inverseMat;

void main()
{
	//how to sample the volumeTexture texture(volumeTexture,TexCoord).r
	//FragColor = vec4(texture(volumeTexture,TexCoord).r,0.0,0.0,1.0);

	FragColor=vec4(gl_FragCoord.x/windowSize.x,gl_FragCoord.y/windowSize.y,gl_FragCoord.w,1.0);

	//current goal, make it turn red when inside
	vec3 cameraRight=normalize(cross(cameraLookDir,vec3(0.0,1.0,0.0)));
	vec3 newCameraPos=(inverseMat*vec4(cameraPos,1.0)).xyz;
	newCameraPos.x+=(voxelSize.x/2.0)/8.0;
	newCameraPos.y+=(voxelSize.y/2.0)/8.0;
	newCameraPos.z+=(voxelSize.z/2.0)/8.0;
	vec3 newCameraDir=(inverseMat*vec4(cameraLookDir,1.0)).xyz;
	vec3 newCameraRight=(inverseMat*vec4(cameraRight,1.0)).xyz;

	if(newCameraPos.x>=0&&newCameraPos.x<voxelSize.x/8.0&&
	newCameraPos.y>=0&&newCameraPos.y<voxelSize.y/8.0&&
	newCameraPos.z>=0&&newCameraPos.z<voxelSize.z/8.0)
	{
	
		FragColor=vec4(1.0,0.0,0.0,1.0);
	}

	//can use uv coords to figure out where on edge

}