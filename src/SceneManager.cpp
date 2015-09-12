
#include <vector>
#include <memory>
#include <cstring>

#include "ResourceManager.h"
#include "Primitive.h"
#include "Camera.h"
#include "Light.h"
#include "Logger.h"

#include "SceneManager.h"
#include "ShaderManager.h"

SceneManager::SceneManager() : camera(45.0f){
    
}

void SceneManager::addLight(Light light){
    light.setID(lights.size());
    lights.push_back(light);
}

void SceneManager::initScene(GLuint &shaderID){

    Logger::Log("Init Planes");
    for(auto &plane : planes){
        plane->Init(shaderID);
    }

    Logger::Log("Init Primitives");
    for(auto &prim : primitives){
        prim->Init(shaderID);
    }

    Logger::Log("Init Lights");
    for(auto &light : lights){
        light.Init(shaderID);
    }

}

void SceneManager::drawScene(std::shared_ptr<ShaderManager> shader){

    cubeMap->setCubeMap();

    // Logger::Log("camera");
    if(camera.animation != NULL) camera.animation->Animate();

    // Logger::Log("Light");
    for(auto light : lights){
        light.Set(camera.getViewMatrix());
    }

    shader->setUniform("shadeMode",0);

    char str[30];
    sprintf(str,"prim num %d\n",primitives.size());
    // Logger::Log(str);
    
    // カメラ行列はあらかじめ送り込んでおく
    glm::mat4 V = camera.getViewMatrix();
    shader->setUniform("V",V);
    shader->setUniform("cameraPosition",camera.transform.position);

    for(auto &prim : primitives){
        // 行列演算と送信
        glm::mat4 M = prim->transform.getModelMatrix();
        glm::mat4 MV = camera.getViewMatrix() * M;
        glm::mat4 MVP = camera.getProjectionMatrix() * MV;

        shader->setUniform("M",M);
        shader->setUniform("MV",MV);
        shader->setUniform("MVP",MVP);

        // 頂点データの送信
        prim->Draw();
    }

    shader->setUniform("shadeMode",1);

    for(auto &plane : planes){
         // 行列演算と送信
        glm::mat4 M = plane->transform.getModelMatrix();
        glm::mat4 MV = camera.getViewMatrix() * M;
        glm::mat4 MVP = camera.getProjectionMatrix() * MV;

        shader->setUniform("M",M);
        shader->setUniform("MV",MV);
        shader->setUniform("MVP",MVP);

        plane->Draw();
    }
    
}

void SceneManager::printScene(){
    Logger::Log("this " + std::to_string((long int)this));
    Logger::Log("printScene");
    Logger::Log("primitive num " + std::to_string(primitives.size()));
    for(auto p : primitives){
        p->print();
    }for(auto l : lights){
        l.print();
    }for(auto p : planes){
        p->print();
    }
}
           



