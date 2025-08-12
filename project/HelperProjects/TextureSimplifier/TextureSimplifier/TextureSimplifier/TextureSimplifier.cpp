#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include "ColorList.h"
#include <vector>;

int min(int a, int b) 
{
    return a < b ? a : b;
}

void updateChunk(int x,int y,unsigned char* imageData,int imageWidth,int imageHeight,int imageChannels, unsigned char* differenceData)
{
    int maxX = min(x + 64, imageWidth);
    int maxY = min(y + 64, imageHeight);
    
    ColorList colorList = ColorList();

    for (int i = x; i < maxX; i++)
    {
        for (int c = y; c < maxY; c++)
        {
            colorList.colors.push_back(imageData[i * imageChannels + c * imageChannels * imageWidth]);
            colorList.colors.push_back(imageData[i * imageChannels + c * imageChannels * imageWidth+1]);
            colorList.colors.push_back(imageData[i * imageChannels + c * imageChannels * imageWidth+2]);
        }
    }

    colorList.removeRepeatFast();

    std::cout << colorList.colors.size() << std::endl;
    std::vector<unsigned char> newColorList = colorList.averageColorList256();

    

    std::cout << "c " << newColorList.size()<< std::endl;
    for (int i = x; i < maxX; i++)
    {
        for (int c = y; c < maxY; c++)
        {
            
           int r=int(imageData[i * imageChannels + c * imageChannels * imageWidth]);
           int g=int(imageData[i * imageChannels + c * imageChannels * imageWidth + 1]);
           int b=int(imageData[i * imageChannels + c * imageChannels * imageWidth + 2]);

           int closestIndex = 1;
           int distance = 60000000;

           for (int d = 1; d < min(256,newColorList.size()/4); d++)
           {
               int deltaR = (r - int(newColorList[d * 4]));
               int deltaG = (g - int(newColorList[d * 4 + 1]));
               int deltaB = (b - int(newColorList[d * 4 + 2]));
               int currentDist = (deltaR * deltaR) +
                   (deltaG * deltaG) +
                   (deltaB * deltaB);
               if (currentDist < distance)
               {
                   distance = currentDist;
                   closestIndex = d;
               }
           }
           differenceData[i * imageChannels + c * imageChannels * imageWidth] = unsigned char(10.0*abs(imageData[i * imageChannels + c * imageChannels * imageWidth] - newColorList[closestIndex * 4]));
           differenceData[i * imageChannels + c * imageChannels * imageWidth+1] = unsigned char(10.0*abs(imageData[i * imageChannels + c * imageChannels * imageWidth+1] - newColorList[closestIndex * 4+1]));
           differenceData[i * imageChannels + c * imageChannels * imageWidth+2] = unsigned char(10.0*abs(imageData[i * imageChannels + c * imageChannels * imageWidth+2] - newColorList[closestIndex * 4+2]));

           imageData[i * imageChannels + c * imageChannels * imageWidth] = newColorList[closestIndex*4];
           imageData[i * imageChannels + c * imageChannels * imageWidth+1] = newColorList[closestIndex*4+1];
           imageData[i * imageChannels + c * imageChannels * imageWidth+2] = newColorList[closestIndex*4+2];
        }
    }
    std::cout << "d" << std::endl;
}


int main()
{
    const char* inputFilePath = "C:\\Users\\breck\\Documents\\LearnOpenGL2\\github\\LearningOpenGL\\project\\LearningOpenGl\\LearningOpenGl\\Textures\\tiedye.jpg";
    const char* outputFilePath = "C:\\Users\\breck\\Documents\\LearnOpenGL2\\github\\LearningOpenGL\\project\\LearningOpenGl\\LearningOpenGl\\Textures\\tiedyeO.png";

    const char* outputDifferenceFilePath = "C:\\Users\\breck\\Documents\\LearnOpenGL2\\github\\LearningOpenGL\\project\\LearningOpenGl\\LearningOpenGl\\Textures\\ODifference.png";

    int imageColorWidth, imageColorHeight,imageColorChannel;
    unsigned char* terrainImageDataColor = stbi_load(inputFilePath, &imageColorWidth, &imageColorHeight, &imageColorChannel, 0);
    unsigned char* terrainImageDifData=new unsigned char[imageColorWidth*imageColorHeight*imageColorChannel];


    std::cout << imageColorWidth << " " << imageColorHeight << " " << imageColorChannel << " " << std::endl;

    for (int x = 0; x < imageColorWidth; x+=64)
    {
        for (int y = 0; y < imageColorHeight; y+=64)
        {
            std::cout << x << " " << y << std::endl;
            updateChunk(x, y, terrainImageDataColor, imageColorWidth, imageColorHeight, imageColorChannel, terrainImageDifData);
        }
    }

    stbi_write_png(outputFilePath, imageColorWidth, imageColorHeight, 3, terrainImageDataColor, imageColorWidth * 3);
    stbi_write_png(outputDifferenceFilePath, imageColorWidth, imageColorHeight, 3, terrainImageDifData, imageColorWidth * 3);

    std::cout << "Hello World!\n";
}
