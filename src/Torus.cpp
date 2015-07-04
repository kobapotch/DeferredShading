
#include <cmath>
#include <iostream>

#include "Primitive.h"

Torus::Torus(Camera* camera) : Primitive(camera){

    float majorStep = 2*M_PI/majorPrecision;
    float minorStep = 2*M_PI/minorPrecision;

    for(int j=0;j<minorPrecision+1;j++){
        for(int i=0;i<majorPrecision+1;i++){

            float theta = majorStep * i;
            float phi = minorStep * j;

            positionData.push_back(
                    (majorRadius - minorRadius*cos(phi) ) * cos(theta)
                    );
            positionData.push_back(
                    (majorRadius - minorRadius*cos(phi) ) * sin(theta)
                    );
            positionData.push_back(
                    minorRadius*sin(phi)
                    );

            normalData.push_back(
                    - cos(phi) * cos(theta)
                    );
            normalData.push_back(
                    - cos(phi) * sin(theta)
                    );
            normalData.push_back(
                    sin(phi)
                    );

            for(int k=0;k<3;k++){
                colorData.push_back(0);
            }

            uvData.push_back(
                    (1.0 / majorPrecision) * i
                    );
            uvData.push_back(
                    (1.0 / minorPrecision) * j
                    );

        }
    }
    // インデックスの作成
    for(int j=0;j<minorPrecision;j++){
        for(int i=0;i<majorPrecision+1;i++){
            indexData.push_back(i + (majorPrecision + 1)*j);
            indexData.push_back(i + (majorPrecision + 1)*(j+1));

        }
    }

}

void Torus::Init(GLuint shaderID){
    Primitive::Init(shaderID);

    glGenBuffers(1,&ibo);

   // 頂点データをOpenGLに登録 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexData.size() * sizeof(GLuint),&indexData[0],GL_STATIC_DRAW);

}

void Torus::DrawVertex(){

    // glDrawArrays(GL_TRIANGLE_STRIP,0,positionData.size());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glDrawElements(GL_TRIANGLE_STRIP,indexData.size(),GL_UNSIGNED_INT,0);


}
