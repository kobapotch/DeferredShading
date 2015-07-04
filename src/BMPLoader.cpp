
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include "BMPLoader.h"
#include "Logger.h"

using namespace std;

const int BMPLoader::unitMacro[] = {
    GL_TEXTURE0,GL_TEXTURE1,GL_TEXTURE2,GL_TEXTURE3};

void BMPLoader::loadBMP(const char* filename){
    char header[54];

    ifstream ifs(filename,ios::binary);
    if(!ifs) {
        cout << "can't open file : " << filename << endl;
        return;
    }

    // ヘッダの読み込み
    ifs.read(header,54);
    if(ifs.bad() || header[0] != 'B' || header[1] != 'M'){
        cout << filename << " is not BMP" << endl;
        return;
    }
    for(int i=0;i<54;i++){
        cout << header[i] ;
    }cout << endl;
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);
    cout << dataPos << " : " << imageSize << " : " << width << " : " << height << endl; 

    char buf;
    while(ifs.get(buf)){
        data.push_back(buf);
    }
    cout << "datasize" << data.size() << " in " << width * height* 3 <<  endl;

}


GLuint BMPLoader::makeTexture(GLuint shaderID,int unit){

    Logger::Log("Load Texture");

    this->unit = unit;

    glActiveTexture(unitMacro[unit]);

    glGenTextures(1,&textureID);

    glBindTexture(GL_TEXTURE_2D,textureID);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,&data[0]);
   /*
    float buf[] = {
        1,0,0, 0,1,0, 0,0,1,
        0,0,0, 1,1,1, 0,0,0,
        0,0,1, 0,1,0, 0,0,1
    };
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,3,3,0,GL_RGB,GL_FLOAT,buf);
*/
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    string uniformstr = string("textures[") + std::to_string(0) + string("]");
    ubo = glGetUniformLocation(shaderID,uniformstr.c_str());

    glBindTexture(GL_TEXTURE_2D,0);

    Logger::Log("ubo is " + std::to_string(ubo));

    return textureID;
}

void BMPLoader::setTexture(){

    glActiveTexture(unitMacro[unit]);
    // GL側のテクスチャを呼ぶ
    glBindTexture(GL_TEXTURE_2D,textureID);
    // GLSL側にGLの持つテクスチャを転送する
    glUniform1i(ubo,unit);
    
}



