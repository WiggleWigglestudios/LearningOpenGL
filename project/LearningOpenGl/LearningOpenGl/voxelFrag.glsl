#version 460 core


out vec4 FragColor;
in vec3 TexCoord;
in vec4 gl_FragCoord;

uniform sampler3D volumeTexture;
uniform vec3 windowSize; //width, height, fov
uniform vec3 cameraPos;
uniform vec3 cameraLookDir;
uniform vec3 voxelSize;
uniform mat4 inverseMat;


struct RayHit
{
	bool hit;
	float hitValue;
    float travelDist;
};

float checkVoxel(vec3 checkPos)
{
	return texture(volumeTexture,checkPos/voxelSize).r;
}

RayHit castRay(vec3 startPos,vec3 rayDir,int maxSteps)
{
	RayHit outputHit;

    outputHit.hit=false;
    outputHit.hitValue=0.0;
	outputHit.travelDist=0.0;

    ivec3 transversePos=ivec3(startPos);
	ivec3 stepDirs = ivec3(sign(rayDir));
    vec3 deltaDist = vec3(rayDir.x==0.0?1e30:abs(1.0/rayDir.x),
                          rayDir.y==0.0?1e30:abs(1.0/rayDir.y),
                          rayDir.z==0.0?1e30:abs(1.0/rayDir.z));
    vec3 stepDists;

	if(rayDir.x<0.0){
        stepDists.x=(startPos.x-float(transversePos.x))*deltaDist.x;
    }else{
        stepDists.x=(float(transversePos.x)+1.0-startPos.x)*deltaDist.x;
    }
    
    if(rayDir.y<0.0){
        stepDists.y=(startPos.y-float(transversePos.y))*deltaDist.y;
    }else{
        stepDists.y=(float(transversePos.y)+1.0-startPos.y)*deltaDist.y;
    }
    
    if(rayDir.z<0.0){
        stepDists.z=(startPos.z-float(transversePos.z))*deltaDist.z;
    }else{
        stepDists.z=(float(transversePos.z)+1.0-startPos.z)*deltaDist.z;
    }

	if(checkVoxel(vec3(transversePos))>0){
          outputHit.hit=true;
          outputHit.hitValue=checkVoxel(transversePos);
          return outputHit;
   
    }
    
    for(int i=0;i<maxSteps;i++){
        if(stepDists.x<stepDists.y){
            if(stepDists.x<stepDists.z){
                outputHit.travelDist=stepDists.x;
                stepDists.x+=deltaDist.x;
                transversePos.x+=stepDirs.x;
                //outputRay.normalDir=vec3(-stepDirs.x,0,0);
                //outputRay.reflectionMult=vec3(-1,1,1);
            }else{
                outputHit.travelDist=stepDists.z;
                stepDists.z+=deltaDist.z;
                transversePos.z+=stepDirs.z;
                //outputRay.normalDir=vec3(0,0,-stepDirs.z);
                //outputRay.reflectionMult=vec3(1,1,-1);
            }
        }else if(stepDists.y<stepDists.z){
            outputHit.travelDist=stepDists.y;
            stepDists.y+=deltaDist.y;
            transversePos.y+=stepDirs.y;
            //outputRay.normalDir=vec3(0,-stepDirs.y,0);
            //outputRay.reflectionMult=vec3(1,-1,1);
        }else{
            outputHit.travelDist=stepDists.z;
            stepDists.z+=deltaDist.z;
            transversePos.z+=stepDirs.z;
            //outputRay.normalDir=vec3(0,0,-stepDirs.z);
            //outputRay.reflectionMult=vec3(1,1,-1);
        }
        
       /* if(travelDist>=maxTravelDist){
            i=maxSteps;
            outputRay.dist=travelDist;
        }
        else*/
        /*
        if(transversePos.x<0||transversePos.x>voxelSize.x||
        transversePos.y<0||transversePos.y>voxelSize.y||
        transversePos.z<0||transversePos.z>voxelSize.z)
        {
            //outputHit.hit=false;
           // i=maxSteps;
        }else*/
        if(checkVoxel(vec3(transversePos))>0){
          i=maxSteps;
          outputHit.hit=true;
          outputHit.hitValue=checkVoxel(transversePos);
        }
       
    }

	return outputHit;
}


void main()
{
    float aspect=windowSize.x/windowSize.y;
    vec2 uv=gl_FragCoord.xy/windowSize.xy;
    uv.x/=aspect;
	//how to sample the volumeTexture texture(volumeTexture,TexCoord).r
	//FragColor = vec4(texture(volumeTexture,TexCoord).r,0.0,0.0,1.0);
	//vec3 safeCoord = clamp(TexCoord, 0.0, 1.0 - 1e-5);
	//FragColor=vec4(floor(safeCoord*voxelSize)/voxelSize,1.0);//vec4(gl_FragCoord.x/windowSize.x,gl_FragCoord.y/windowSize.y,gl_FragCoord.w,1.0);

	//current goal, make it turn red when inside
	vec3 cameraRight=normalize(cross(cameraLookDir,vec3(0.0,1.0,0.0)));
	vec3 newCameraPos=(inverseMat*vec4(cameraPos,1.0)).xyz;
	newCameraPos.x+=(voxelSize.x/2.0)/8.0;
	newCameraPos.y+=(voxelSize.y/2.0)/8.0;
	newCameraPos.z+=(voxelSize.z/2.0)/8.0;
	vec3 newCameraDir=(inverseMat*vec4(cameraLookDir,1.0)).xyz;
	vec3 newCameraRight=(inverseMat*vec4(cameraRight,1.0)).xyz;

	
	vec3 safeCoord = clamp(TexCoord, 0.0, 1.0 - 1e-5);
	vec3 startingPos=safeCoord*voxelSize;//floor(safeCoord*voxelSize)/voxelSize;

	if(newCameraPos.x>=0&&newCameraPos.x<voxelSize.x/8.0&&
	newCameraPos.y>=0&&newCameraPos.y<voxelSize.y/8.0&&
	newCameraPos.z>=0&&newCameraPos.z<voxelSize.z/8.0)
	{
	
		startingPos=newCameraPos;
	}

	
	FragColor=vec4(startingPos,1.0);
	int maxSteps=int(voxelSize.x)+int(voxelSize.y)+int(voxelSize.z)+1;
    



    float halfHeight = tan(windowSize.z / 2.0f);
    float halfWidth = halfHeight * aspect;

    // 2. Convert uv to screen space in [-1, 1] range
    //    where (0, 0) = bottom-left and (1, 1) = top-right
    float sx = uv.x * 2.0f - aspect;
    float sy = uv.y * 2.0f - 1.0f;

    vec3 right=normalize(cross(newCameraDir,vec3(0.0,1.0,0.0)));
    vec3 up=normalize(cross(right,newCameraDir));

	vec3 rayDir=normalize(newCameraDir+sx * halfWidth * right + sy * halfHeight * up);

	RayHit testHit=castRay(startingPos,rayDir,maxSteps);
	FragColor=vec4(startingPos/voxelSize,1.0);
    FragColor=vec4(rayDir,1.0);

    
    FragColor.x=1.0/(length(newCameraPos-startingPos/8.0)+testHit.travelDist);
    FragColor.y=1.0/(length(newCameraPos-startingPos/8.0)+testHit.travelDist);
    FragColor.z=1.0/(length(newCameraPos-startingPos/8.0)+testHit.travelDist);

    if(!testHit.hit)
    {
        discard;
        //FragColor=vec4(0.0,0.0,0.0,1.0);
    }
}