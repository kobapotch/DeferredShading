
#include "Primitive.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <random>

#include "Logger.h"



Primitive::Primitive(Camera* m_camera){
    camera = m_camera;
}
Primitive::~Primitive(){
    if(animation == NULL) delete animation;
}
/*
Primitive::Primitive(const Primitive& obj){
    for(int i=0;i<VERTEX_INFO_NUM;i++){
        vbo[i] = obj.vbo[i];
    }
    vao = obj.vao;
    ubo = obj.ubo;

    camera = obj.camera;
    textureID = obj.textureID;

    positionData = obj.positionData;
    colorData = obj.colorData;

}
*/

void Primitive::Init(GLuint shaderID){
    Logger::Log("Primitive Init");
   
    // VAOの作成とバインド
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    cout << "VAO is " << vao << endl;

    glGenBuffers(VERTEX_INFO_NUM,vbo);
    GLuint positionBufferHandle = vbo[0];
    GLuint colorBufferHandle = vbo[1];
    GLuint normalBufferHandle = vbo[2];
    GLuint uvBufferHandle = vbo[3];

   // 頂点データをOpenGLに登録 
    glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,positionData.size() * sizeof(GLfloat),&positionData[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,colorData.size() * sizeof(GLfloat),&colorData[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,normalData.size() * sizeof(GLfloat),&normalData[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,uvBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,uvData.size() * sizeof(GLfloat) , &uvData[0],GL_STATIC_DRAW);

    // 配列属性の有効化とバッファへの対応付け
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER,positionBufferHandle);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,normalBufferHandle);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,uvBufferHandle);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(void*)0);

    // uniform変数の関連付け
    ubo.push_back(glGetUniformLocation(shaderID,"MVP"));
    ubo.push_back(glGetUniformLocation(shaderID,"MV"));
    ubo.push_back(glGetUniformLocation(shaderID,"M"));
    ubo.push_back(glGetUniformLocation(shaderID,"V"));
    ubo.push_back(glGetUniformLocation(shaderID,"cameraPosition"));

    glBindVertexArray(0);

    if(indexData.size() != 0){
        glGenBuffers(1,&ibo);

        // 頂点データをOpenGLに登録 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexData.size() * sizeof(GLuint),&indexData[0],GL_STATIC_DRAW);
    }

}

float angle = 0;

void Primitive::Draw(GLuint shaderID){

    if(animation != NULL) {
        animation->Animate();
    }
    glm::mat4 M = transform.getModelMatrix();
    glm::mat4 MV = camera->getViewMatrix() * M;
    glm::mat4 MVP = camera->getProjectionMatrix() * MV;
    glm::mat4 V = camera->getViewMatrix();

    glUseProgram(shaderID);
    glBindVertexArray(vao);

    // テクスチャの切り替え
    if(texture != NULL) texture->setTexture();
    // マテリアルのセット
    if(material != nullptr) material->Set();

    // 変換行列をシェーダーに送り込む
    glUniformMatrix4fv(ubo[0],1,GL_FALSE,&MVP[0][0]);
    glUniformMatrix4fv(ubo[1],1,GL_FALSE,&MV[0][0]);
    glUniformMatrix4fv(ubo[2],1,GL_FALSE,&M[0][0]);
    glUniformMatrix4fv(ubo[3],1,GL_FALSE,&V[0][0]);
    glUniform3fv(ubo[4],1,&camera->getPosition()[0]);

    // プリミティブによって描画方法を変えられる
    DrawVertex();

    glBindTexture(GL_TEXTURE_2D,0);

    glBindVertexArray(0);

}


// デフォルトの描画方法
void Primitive::DrawVertex(){

    if(indexData.size() == 0){
        glDrawArrays(GL_TRIANGLE_STRIP,0,positionData.size());
    }
    else{
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
        glDrawElements(GL_TRIANGLES,indexData.size(),GL_UNSIGNED_INT,0);
    }
}
