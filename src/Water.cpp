
#include "Primitive.h"




Water::Water(Camera* camera) : Primitive(camera){
    float xStep = width / precision;
    float zStep = height / precision;

    // 周波数と振幅
    float w = 0.5;
    float h = 1;

    for(int i=0;i<=precision;++i){
        for(int j=0;j<=precision;++j){

            positionData.push_back(-width/2.0 + i*xStep);
            positionData.push_back(h*sin(w*i*xStep));
            positionData.push_back(-height/2.0 + j*zStep);

            colorData.push_back(0);
            colorData.push_back(0);
            colorData.push_back(0);

            normalData.push_back(-w*cos(w*i*xStep));
            normalData.push_back(1);
            normalData.push_back(0);

            uvData.push_back(i);
            uvData.push_back(j);
        }
    }

    for(int i=0;i<precision;++i){
        // indexData.push_back(i*precision);
        for(int j=0;j<precision;++j){
            indexData.push_back(i*(precision)+j);
            indexData.push_back((i+1)*precision + j);
        }
    }
}

void Water::Init(GLuint shaderID){
    Primitive::Init(shaderID);

    glGenBuffers(1,&ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexData.size()*sizeof(GLuint),&indexData[0],GL_STATIC_DRAW);
}

void Water::DrawVertex(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glDrawElements(GL_TRIANGLE_STRIP,indexData.size(),GL_UNSIGNED_INT,0);
}


