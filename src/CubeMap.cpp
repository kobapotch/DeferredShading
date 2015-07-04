
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

#include "CubeMap.h"
#include "Logger.h"

using namespace std;

const int CubeMap::unitMacro[] = {
    GL_TEXTURE0,GL_TEXTURE1,GL_TEXTURE2,GL_TEXTURE3,GL_TEXTURE4};

void CubeMap::loadCubeTex(const char* posx,const char* negx,
        const char* posy,const char* negy,
        const char* posz, const char* negz){

    const char* filenames[] = { posx,negx,posy,negy,posz,negz};

    Logger::Log("Load CubeMap");

    for(int i=0;i<6;i++){

        image[i] = cv::imread(filenames[i],1);
        if(image[i].data==NULL){
            cout << "imagefile read error in " << filenames[i] << endl;
            return;
        }
        cout << "image " << filenames[i] << " loaded" << endl;
        cout << "width:" << image[i].cols << " height:" << image[i].rows << endl;

    }

}

void CubeMap::makeCubeMap(unsigned int shaderID,int unit){

    GLuint targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    Logger::Log("Make CubeMap");

    this->unit = unit;

    glActiveTexture(unitMacro[unit]);


    glGenTextures(1,&textureID);

    glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);

    for(int i=0;i<6;i++){
        if(image[i].elemSize() == 3){
            Logger::Log("RGB image");
            glTexImage2D(targets[i],0,GL_RGB,image[i].cols,image[i].rows,0,GL_BGR,GL_UNSIGNED_BYTE,image[i].data);
        }else if(image[i].elemSize() == 4){
            Logger::Log("RGBA image");
            glTexImage2D(targets[i],0,GL_RGBA,image[i].cols,image[i].rows,0,GL_BGRA,GL_UNSIGNED_BYTE,image[i].data);
        }


    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

    ubo = glGetUniformLocation(shaderID,"cubeMap");

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    Logger::Log("ubo is " + std::to_string(ubo));
}

void CubeMap::setCubeMap(){

    glActiveTexture(unitMacro[unit]);
    glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);
    glUniform1i(ubo,unit);
}



