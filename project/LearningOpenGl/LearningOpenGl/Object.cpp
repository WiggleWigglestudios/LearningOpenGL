#include "Object.h"
#include "ogt_vox.h";

Object::Object(glm::vec3 spawnPos, glm::vec3 spawnForwardDir, glm::vec3 spawnUpDir, std::vector<unsigned char> _voxelData,
	glm::i8vec3 _voxelSize, std::vector<unsigned char> _voxelPalatte)
{
	pos = spawnPos;
	forwardDir = spawnForwardDir;
	upDir = spawnUpDir;
	voxelData.resize(_voxelData.size());
	for(int i=0;i<voxelData.size();i++)
	{ 
		voxelData[i] = _voxelData[i];
	}
	voxelPalatte.resize(_voxelPalatte.size());
	for (int i = 0; i < voxelPalatte.size(); i++)
	{
		voxelPalatte[i] = _voxelPalatte[i];
	}
	voxelSize = _voxelSize;
}



Object::Object(glm::vec3 spawnPos, glm::vec3 spawnForwardDir, glm::vec3 spawnUpDir, std::string fileName, int modelNumber)
{
	pos = spawnPos;
	forwardDir = spawnForwardDir;
	upDir = spawnUpDir;
	std::size_t voxelFileDataSize = 0;
	uint8_t* voxelFileData = loadFileBytes(fileName, voxelFileDataSize);
	if (voxelFileDataSize >0)
	{
		const ogt_vox_scene* scene = ogt_vox_read_scene(voxelFileData, voxelFileDataSize);
		if (modelNumber < scene->num_models)
		{
			const ogt_vox_model* model= scene->models[modelNumber];
			int xSize = model->size_x;
			int ySize = model->size_y;
			int zSize = model->size_z;
			voxelData.resize(xSize * ySize * zSize);
			std::cout << xSize <<" " << ySize<<" " << zSize <<" " <<voxelData.size()<< std::endl;
			//std::vector<unsigned char> voxelData(xSize * ySize * zSize, 0);
			int voxelIndex = 0;
			for (int z = 0; z < zSize; z++)
			{
				for (int y = 0; y < ySize; y++)
				{
					for (int x = 0; x < xSize; x++, voxelIndex++)
					{
						 voxelIndex = (x) % xSize +
							 ((y) % ySize)*xSize+
							 ((z) % zSize)*xSize*ySize;
						 voxelData[x + (y * xSize) + (z * xSize * ySize)]= model->voxel_data[voxelIndex];
						 if (voxelData[x + (y * xSize) + (z * xSize * ySize)] == 1)
						 {
							 voxelData[x + (y * xSize) + (z * xSize * ySize)] = 252;
						 }

					}
				}
			}
			voxelPalatte.resize(256 * 4);
			//std::vector<unsigned char> voxelPalatte(256 * 4, 0);
			for (int i = 0; i < 256; i++)
			{
				voxelPalatte[i * 4 + 0] = int(scene->palette.color[i].r);
				voxelPalatte[i * 4 + 1] = int(scene->palette.color[i].g);
				voxelPalatte[i * 4 + 2] = int(scene->palette.color[i].b);
				voxelPalatte[i * 4 + 3] = 255;
			}

			voxelPalatte[252 * 4 + 0] = 90;// voxelPalatte[1 * 4 + 0];
			voxelPalatte[252 * 4 + 1] = 140;// voxelPalatte[1 * 4 + 1];
			voxelPalatte[252 * 4 + 2] = 255;// voxelPalatte[1 * 4 + 2];

			ogt_vox_destroy_scene(scene);

			voxelSize = glm::i8vec3(xSize, ySize, zSize);
		}
		else
		{
			std::cout << "can't load model " <<modelNumber<<" from " << fileName << std::endl;
		}
	}
	else
	{
		std::cout << "can't load " << fileName << std::endl;
	}
}



void Object::updateVolumeTexture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &volumeTexture);
	glBindTexture(GL_TEXTURE_3D, volumeTexture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);

	if (voxelData.size() == voxelSize.x * voxelSize.y * voxelSize.z)
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, voxelSize.x, voxelSize.y, voxelSize.z,
			0, GL_RED, GL_UNSIGNED_BYTE, voxelData.data());
	}
	else
	{
		std::cout << "incorrect voxel data size. can't update volume texture" << std::endl;
		std::cout << voxelData.size() << " vs the expected " << voxelSize.x * voxelSize.y * voxelSize.z << std::endl;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Object::updatePaletteTexture()
{
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &paletteTexture);
	glBindTexture(GL_TEXTURE_2D, paletteTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	if (voxelPalatte.size() == 256*4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1,
			0, GL_RGBA, GL_UNSIGNED_BYTE, voxelPalatte.data());
	}
	else
	{
		std::cout << "incorrect palette data size. can't update palette texture" << std::endl;
		std::cout << voxelPalatte.size() << " vs the expected " << (256*4) << std::endl;
	}
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Object::createVertexBufferObject()
{
    float vertices[] = {
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,0.0f,
        -0.5f,-0.5f,0.5f,   0.0f,0.0f,1.0f,
        -0.5f,0.5f,-0.5f,   0.0f,1.0f,0.0f,
        -0.5f,0.5f,0.5f,    0.0f,1.0f,1.0f,
        0.5f,-0.5f,-0.5f,   1.0f,0.0f,0.0f,
        0.5f,-0.5f,0.5f,    1.0f,0.0f,1.0f,
        0.5f,0.5f,-0.5f,    1.0f,1.0f,0.0f,
        0.5f,0.5f,0.5f,     1.0f,1.0f,1.0f,
    };
    unsigned int indices[] = {
        //back face
        0,2,6,
		6,4,0,
        //front face
        5,7,3,
		5,3,1,
        //bottom face
		0,5,1,
		0,4,5,
        //top face
		6,2,3,
		6,3,7,
        //left face
        2,0,1,
		2,1,3,
        //right face
        4,6,7,
		4,7,5
    };

	glGenBuffers(1, &VBO);

	
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//first parameter tells which vertex attribute we are configuring. In our case vertex 
   //attribute 0 is a vec3 position (defined in the ver.glsl shader)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Object::updateShader(Shader inputShader)
{
    voxelShader = inputShader;
}


void Object::rotate(float angle, glm::vec3 axis)
{
    forwardDir = glm::vec3(glm::vec4(forwardDir, 1.0) * glm::rotate(glm::mat4(1.0), angle, axis));
    upDir = glm::vec3(glm::vec4(upDir, 1.0) * glm::rotate(glm::mat4(1.0), angle, axis));
}


glm::mat4 Object::generateRotationMatrix()
{

	// Compute right vector
	glm::vec3 right = glm::normalize(glm::cross(-forwardDir, upDir));

	// Recompute orthogonal up direction
	glm::vec3 upOrtho = glm::cross(right, -forwardDir);

	glm::mat4 rotationMat = glm::mat4(1.0);//glm::lookAt(glm::vec3(0, 0, 0),-forwardDir, upDir);
	rotationMat[0] = glm::vec4(right, 0.0);
	rotationMat[1] = glm::vec4(upOrtho, 0.0);
	rotationMat[2] = glm::vec4(forwardDir, 0.0);

	return rotationMat;
}

glm::mat4 Object::generateTranslationMatrix()
{
	return glm::translate(glm::mat4(1.0), pos);
}

void Object::render(glm::mat4 viewMat,glm::mat4 projectionMat)
{

    voxelShader.use();
	voxelShader.setInt("volumeTexture", 0);
	voxelShader.setInt("paletteTexture", 1);



	glm::mat4 modelMat = generateTranslationMatrix() * generateRotationMatrix() * glm::scale(glm::mat4(1.0), glm::vec3(voxelSize)*glm::vec3(1.0/8.0,1.0/8.0,1.0/8.0));

	glm::mat4 inverseMat = glm::inverse(generateTranslationMatrix() * generateRotationMatrix());// glm::translate(glm::mat4(1.0), pos)* glm::lookAt(glm::vec3(0, 0, 0), forwardDir, upDir));
	//inverseMat = glm::mat4(1.0);
	glm::mat4 inverseRotMat = glm::inverse(generateRotationMatrix());// glm::translate(glm::mat4(1.0), pos)* glm::lookAt(glm::vec3(0, 0, 0), forwardDir, upDir));

    voxelShader.setMat4("model", modelMat);
    voxelShader.setMat4("view", viewMat);
	voxelShader.setMat4("projection", projectionMat);
	voxelShader.setMat4("inverseMat", inverseMat);
	voxelShader.setMat4("inverseRotMat", inverseRotMat);
	voxelShader.setVec3("voxelSize", float(voxelSize.x), float(voxelSize.y), float(voxelSize.z));
    

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, volumeTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, paletteTexture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * 2 * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


