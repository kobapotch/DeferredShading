
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>

#include "Texture.h"
#include "Logger.h"

using namespace std;

const int Texture::unitMacro[] = {
    GL_TEXTURE0,GL_TEXTURE1,GL_TEXTURE2,GL_TEXTURE3};

void Texture::loadTexture(const char* filename){

    image = cv::imread(filename,-1);
    if(image.data==NULL){
        cout << "imagefile read error in " << filename << endl;
        return;
    }

}


GLuint Texture::makeTexture(GLuint shaderID,int unit){

    Logger::Log("Load Texture");

    this->unit = unit;

    glActiveTexture(unitMacro[unit]);

    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);
    
    if(image.elemSize() == 3){
        cout << "RGB texture" << endl;
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,image.cols,image.rows,0,GL_RGB,GL_UNSIGNED_BYTE,&image.data[0]);
    }else if(image.elemSize()==4){
        cout << "RGBA texture" << endl;
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.cols,image.rows,0,GL_BGRA,GL_UNSIGNED_BYTE,&image.data[0]);
    }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    string uniformstr = string("textures[") + std::to_string(0) + string("]");
    ubo = glGetUniformLocation(shaderID,uniformstr.c_str());

    glBindTexture(GL_TEXTURE_2D,0);

    Logger::Log("ubo is " + std::to_string(ubo));

    return textureID;
}

void Texture::setTexture(){

    glActiveTexture(unitMacro[unit]);
    // GL側のテクスチャを呼ぶ
    glBindTexture(GL_TEXTURE_2D,textureID);
    // GLSL側にGLの持つテクスチャを転送する
    glUniform1i(ubo,unit);
    
}



