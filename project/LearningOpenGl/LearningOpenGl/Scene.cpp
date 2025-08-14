#include "stb_image.h"

#include "Scene.h"


Scene::Scene(const std::string& _sceneFilePath)
{
	sceneFilePath = _sceneFilePath;
}

int min(int a, int b)
{
    return a < b ? a : b;
}


void CreateChunkInIndexRange(std::vector<Object>& objects, int startIndex, int endIndex,unsigned char* terrainImageData,int heightMapImageWidth,int heightMapImageHeight,int heightMapImageChannel,
    unsigned char* terrainImageDataColor,int terrainImageColorChannel, int ChunkXSize, float ChunkYSize,int ChunkZSize,Shader& voxelShader,int MapYSize)
{
    for (int i = startIndex; i < endIndex; i++)
    {
        int x = i/(MapYSize / ChunkZSize);
        int y = i%(MapYSize / ChunkZSize);
        std::cout << "Loading chunk:" << x << "," << y << std::endl;
        Object terrainObject = Object(glm::vec3(x * 8, 0, y * 8), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0),
            terrainImageData, heightMapImageWidth, heightMapImageHeight, heightMapImageChannel, 0,
            terrainImageDataColor, terrainImageColorChannel, x * ChunkXSize, y * ChunkZSize, ChunkYSize);//64

        terrainObject.updateShader(voxelShader);
        terrainObject.updateVolumeTexture();
        terrainObject.updatePaletteTexture();
        terrainObject.createVertexBufferObject();
        objects[i] = (terrainObject);
    }
}

/* Example file
Textures\\terrainTest1DepthMap.png  //height map file path
Textures\\terrainTest1DepthMap.png  //color map file path
1024,1024 //level size int,int
64,100.0,64 //terrain chunk size int,float,int
3 //model count
1,2,3,0,0,1,0,1,0,0 object pos x,y,z forward dir x,y,z up dir x,y,z, model number n
vox\\generic_sedan_red.vox
-0.375,7.625,10.125,0,1,0,0,0,-1,0 object pos x,y,z forward dir x,y,z up dir x,y,z, model number n
vox\\tree_birch.vox
0,0,10,0,1,0,0,0,-1,1 object pos x,y,z forward dir x,y,z up dir x,y,z, model number n
vox\\tree_birch.vox
*/
void Scene::Load()
{

	std::ifstream SceneFile;
    SceneFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // open files
        SceneFile.open(sceneFilePath);
        std::stringstream sceneStream;
        // read file's buffer contents into streams
        sceneStream << SceneFile.rdbuf();
        // close file handlers
        SceneFile.close();
        // convert stream into string
        sceneText = sceneStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SCENE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    //std::cout << sceneText << std::endl;
    
    int startIndex = 0;
    int endIndex = 0;
    endIndex=sceneText.find('\n', startIndex);
    std::string HeightMapPath = sceneText.substr(startIndex, endIndex- startIndex);
    std::cout <<"Height Map Path:'" << HeightMapPath<<"'" << std::endl;

    startIndex = endIndex+1;
    endIndex = sceneText.find('\n', startIndex);
    std::string ColorMapPath = sceneText.substr(startIndex, endIndex- startIndex);
    std::cout <<"Color Map Path:'" << ColorMapPath << "'" << std::endl;
   

    startIndex = endIndex+1;
    endIndex = sceneText.find(',', startIndex);
    int MapXSize = std::stoi(sceneText.substr(startIndex, endIndex - startIndex));
    std::cout << MapXSize<<" x size" << std::endl;

    startIndex = endIndex + 1;
    endIndex = sceneText.find('\n', startIndex);
    int MapYSize = std::stoi(sceneText.substr(startIndex, endIndex - startIndex));
    std::cout << MapYSize << " y size" << std::endl;

    
    startIndex = endIndex + 1;
    endIndex = sceneText.find(',', startIndex);
    int ChunkXSize = std::stoi(sceneText.substr(startIndex, endIndex - startIndex));
    std::cout << ChunkXSize << " chunk x size" << std::endl;

    startIndex = endIndex + 1;
    endIndex = sceneText.find(',', startIndex);
    float ChunkYSize = std::stof(sceneText.substr(startIndex, endIndex - startIndex));
    std::cout << ChunkYSize << " chunk y size" << std::endl;

    startIndex = endIndex + 1;
    endIndex = sceneText.find('\n', startIndex);
    int ChunkZSize = std::stoi(sceneText.substr(startIndex, endIndex - startIndex));
    std::cout << ChunkZSize << " chunk z size" << std::endl;

    startIndex = endIndex + 1;
    endIndex = sceneText.find('\n', startIndex);
    int ModelCount = std::stoi(sceneText.substr(startIndex, endIndex - startIndex));
    std::cout << ModelCount << " Models" << std::endl;











    int heightMapImageWidth, heightMapImageHeight, heightMapImageChannel;
    unsigned char* terrainImageData = stbi_load(HeightMapPath.c_str(), &heightMapImageWidth, &heightMapImageHeight, &heightMapImageChannel, 0);

    int terrainImageColorWidth, terrainImageColorHeight, terrainImageColorChannel;
    unsigned char* terrainImageDataColor = stbi_load(ColorMapPath.c_str(), &terrainImageColorWidth, &terrainImageColorHeight, &terrainImageColorChannel, 0);
   
    if (terrainImageColorWidth != heightMapImageWidth || terrainImageColorHeight != heightMapImageHeight)
    {
        std::cout << "images not same size" << std::endl;
    }



    voxelShader = Shader("voxelVert.glsl", "voxelFrag.glsl");

    MapXSize = min(MapXSize, heightMapImageWidth);
    MapYSize = min(MapYSize, heightMapImageHeight);
 
    //std::cout << "chunk sizes: " << ChunkXSize << "," << ChunkYSize << ", " << ChunkZSize << std::endl;

    objects.resize((MapXSize / ChunkXSize) * (MapYSize / ChunkZSize) + ModelCount);


    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Hardware concurrency: " << n << " threads\n";
    n--;
    if (n > 1)
    { 
        int totalChunks = MapXSize / ChunkXSize * MapYSize / ChunkZSize;
        CreateChunkInIndexRange(objects, 0, totalChunks, terrainImageData, heightMapImageWidth, heightMapImageHeight, heightMapImageChannel, terrainImageDataColor,
            terrainImageColorChannel, ChunkXSize, ChunkYSize, ChunkZSize, voxelShader, MapYSize);
       // std::thread t1(CreateChunkInIndexRange, objects, 0, totalChunks/2, terrainImageData, heightMapImageWidth, heightMapImageHeight, heightMapImageChannel, terrainImageDataColor,
        //        terrainImageColorChannel, ChunkXSize, ChunkYSize, ChunkZSize, voxelShader, MapYSize);
       // std::thread t2(CreateChunkInIndexRange, objects, totalChunks / 2, totalChunks, terrainImageData, heightMapImageWidth, heightMapImageHeight, heightMapImageChannel, terrainImageDataColor,
        //    terrainImageColorChannel, ChunkXSize, ChunkYSize, ChunkZSize, voxelShader, MapYSize);
        //t1.join();
        //t2.join();
    }
    else
    {
        int voxelCount = 0;
        int objectOn = 0;
        for (int x = 0; x < MapXSize / ChunkXSize; x++)
        {
            for (int y = 0; y < MapYSize / ChunkZSize; y++)
            {
                Object terrainObject = Object(glm::vec3(x * 8, 0, y * 8), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0),
                    terrainImageData, heightMapImageWidth, heightMapImageHeight, heightMapImageChannel, 0,
                    terrainImageDataColor, terrainImageColorChannel, x * ChunkXSize, y * ChunkZSize, ChunkYSize);//64

                terrainObject.updateShader(voxelShader);
                terrainObject.updateVolumeTexture();
                terrainObject.updatePaletteTexture();
                terrainObject.createVertexBufferObject();
                objects[objectOn] = (terrainObject);
                objectOn++;
                voxelCount += terrainObject.voxelSize.x * terrainObject.voxelSize.y * terrainObject.voxelSize.z;
                std::cout << "Loading chunk:" << x << "," << y << std::endl;
            }
        }
        std::cout << "total voxels in terrain: " << voxelCount << std::endl;
    }
}



void Scene::Render(glm::mat4 viewMat, glm::mat4 projectionMat, glm::vec3 playerPos, glm::vec3 playerLookDir, glm::vec4 windowInfo)
{

    objects[0].voxelShader.use();
    objects[0].voxelShader.setVec4("windowSize", windowInfo.x, windowInfo.y, windowInfo.z, windowInfo.w);//float(width), float(height), fov, far);
    objects[0].voxelShader.setVec3("cameraPos", playerPos.x, playerPos.y, playerPos.z);// player.pos.x, player.pos.y, player.pos.z);// cubePos.x, cubePos.y, cubePos.z);
    objects[0].voxelShader.setVec3("cameraLookDir", playerLookDir.x, playerLookDir.y, playerLookDir.z);// player.lookDir.x, player.lookDir.y, player.lookDir.z);

    for (int i = 0; i < objects.size(); i++)
    {
        objects[i].render(viewMat, projectionMat);
    }
}