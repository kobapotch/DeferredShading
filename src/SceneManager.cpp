
#include <vector>
#include <memory>

#include "ResourceManager.h"
#include "Primitive.h"
#include "Camera.h"
#include "Light.h"
#include "Logger.h"

#include "SceneManager.h"

SceneManager::SceneManager(ResourceManager* resourceManager) : camera(45.0f){
    // 背景平面の準備
    Plane plane(&camera);

    std::shared_ptr<Material> mat = resourceManager->materials[1];
    std::shared_ptr<Texture> texture = resourceManager->textures[0];

    // 奥
    std::shared_ptr<Plane> planeBack(new Plane(&camera));
    planeBack->texture = texture.get();
    planeBack->material = mat;
    planeBack->transform.scale = glm::vec3(40,40,1);
    planeBack->transform.position = glm::vec3(0,10,-20);
    planes.push_back(planeBack);

    // 左
    std::shared_ptr<Plane> planeLeft(new Plane(&camera));
    planeLeft->texture = texture.get();
    planeLeft->material = mat;
    planeLeft->transform.scale = glm::vec3(40,40,1);
    planeLeft->transform.position = glm::vec3(-20,10,0);
    planeLeft->transform.rotation = glm::vec3(0,90,0);
    planes.push_back(planeLeft);

    // 下
    std::shared_ptr<Plane> planeButtom(new Plane(&camera));
    planeButtom->texture = texture.get();
    planeButtom->material = mat;
    planeButtom->transform.scale = glm::vec3(100,100,1);
    planeButtom->transform.position = glm::vec3(0,-10,0);
    planeButtom->transform.rotation = glm::vec3(-90,0,0);
    // planes.push_back(planeButtom);
    
    std::shared_ptr<Water> wat(new Water(&camera));
    water = wat;
    water->transform.position = glm::vec3(0,-10,0);
    water->texture = resourceManager->textures[0].get();
    water->material = resourceManager->materials[1];


    // 水面反射用フレームバッファの作成
    glGenFramebuffers(1,&fbo);
    glBindFramebuffer(GL_FRAMEBUFFER,fbo);

    // フレーム描画対象のテクスチャ スロット３を使用
    glGenTextures(1,&renderTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,renderTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1024,1024,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // テクスチャをフレームバッファにバインド
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTex,0);

    // 深度バッファの作成
    glGenRenderbuffers(1,&fboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER,fboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,1024,1024);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,fboDepth);

    // フラグメントシェーダーの出力ターゲットを設定
    GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,drawBufs);

    // フレームバッファへのバインドを解除すると，デフォルトのフレームバッファに対象が戻る
    glBindFramebuffer(GL_FRAMEBUFFER,0);


    // 書き込むテクスチャの準備
    GLubyte whiteTex[] = {255,255,255,0,
    255,255,255,0,
    255,255,255,0,
    255,255,255,0
    };
    glActiveTexture(GL_TEXTURE4);
    glGenTextures(1,&whiteTexHandle);
    glBindTexture(GL_TEXTURE_2D,whiteTexHandle);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,2,2,0,GL_RGBA,GL_UNSIGNED_BYTE,whiteTex);
    

}

void SceneManager::addLight(Light light){
    light.setID(lights.size());
    lights.push_back(light);
}

void SceneManager::initScene(GLuint &shaderID){
    glClearColor(0.1f,0.1f,0.2f,1.0f);

    Logger::Log("Init Planes");
    for(auto &plane : planes){
        plane->Init(shaderID);
    }
    water->Init(shaderID);

    Logger::Log("Init Primitives");
    for(auto &prim : primitives){
        prim->Init(shaderID);
    }

    Logger::Log("Init Lights");
    for(auto &light : lights){
        light.Init(shaderID);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void SceneManager::drawScene(GLuint &shaderID){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderID);


    cubeMap->setCubeMap();

    if(camera.animation != NULL) camera.animation->Animate();

    glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    glViewport(0,0,1024,1024);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE3);
    glBindBuffer(GL_TEXTURE_2D,renderTex);
    int loc = glGetUniformLocation(shaderID,"fbo");

    camera.setInvert(true);

    for(auto light : lights){
        light.Set(camera.getViewMatrix());
    }
    for(auto &plane : planes){
        plane->Draw(shaderID);
    }
    for(auto &prim : primitives){
        prim->Draw(shaderID);
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(0,0,1280*2,800*2);

    glUniform1i(loc,3);
    camera.setInvert(false);

     for(auto light : lights){
        light.Set(camera.getViewMatrix());
    }

    for(auto &plane : planes){
        plane->Draw(shaderID);
    }

    for(auto &prim : primitives){
        prim->Draw(shaderID);
    }
    if(water == NULL) {Logger::Log("error");}
    water->Draw(shaderID);
    
}
