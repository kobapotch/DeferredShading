#include <cmath>
#include <iostream>

#include "Primitive.h"

Sphere::Sphere(Camera* camera) : Primitive(camera){

    float horStep = M_PI/precision;
    float verStep = M_PI/precision;

    for(int i=0;i<precision+1;i++){
        for(int j=0;j<precision*2+1;j++){

            float verAngle = verStep * i;
            float horAngle = horStep * j;

            positionData.push_back(radius*sin(verAngle)*cos(horAngle));
            positionData.push_back(radius*sin(verAngle)*sin(horAngle));
            positionData.push_back(radius*cos(verAngle));

            colorData.push_back(0);
            colorData.push_back(0);
            colorData.push_back(0);

            normalData.push_back(sin(verAngle) * cos(horAngle));
            normalData.push_back(sin(verAngle) * sin(horAngle));
            normalData.push_back(cos(verAngle));

            uvData.push_back(0.5/(precision) * j);
            uvData.push_back(1.0/(precision) * i);
        }
    }

    for(int i=0;i<precision+1;i++){
        for(int j=0;j<precision*2+1;j++){
            indexData.push_back(i*(precision*2+1) + j);
            indexData.push_back((i+1)*(precision*2+1) + j);
        }
    }

}

void Sphere::Init(GLuint shaderID){
    Primitive::Init(shaderID);

    glGenBuffers(1,&ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexData.size() * sizeof(GLuint),&indexData[0],GL_STATIC_DRAW);
}

void Sphere::DrawVertex(){

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glDrawElements(GL_TRIANGLE_STRIP,indexData.size(),GL_UNSIGNED_INT,0);
}
