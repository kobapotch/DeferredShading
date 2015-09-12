#include "Renderer.h"

Renderer::Renderer(uint width,uint height){
    this->width = width;
    this->height = height;
}

void Renderer::makeBuffers(){

    int MaxUnitNum = -1;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&MaxUnitNum);
    std::cout <<"最大テクスチャユニット数 : "<< MaxUnitNum << "\n";
    
    glGenFramebuffers(1,&deferredFBO);
    glBindFramebuffer(GL_FRAMEBUFFER,deferredFBO);

    // 深度バッファ
    glGenRenderbuffers(1,&depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER,depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);

    // 位置バッファ
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1,&posTex);
    glBindTexture(GL_TEXTURE_2D,posTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    positionLoc = glGetUniformLocation(shader->programID,"PositionTex");

    // 法線バッファ
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1,&normTex);
    glBindTexture(GL_TEXTURE_2D,normTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB32F,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    normalLoc = glGetUniformLocation(shader->programID,"NormalTex");

    // 反射率バッファ
    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1,&colorTex);
    glBindTexture(GL_TEXTURE_2D,colorTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    colorLoc = glGetUniformLocation(shader->programID,"ColorTex");

    // マテリアル情報バッファ
    glActiveTexture(GL_TEXTURE3);
    glGenTextures(1,&materialTex);
    glBindTexture(GL_TEXTURE_2D,materialTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    materialLoc = glGetUniformLocation(shader->programID,"MaterialTex");

    // フレームバッファにアタッチする
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuf);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,posTex,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,normTex,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,colorTex,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT3,GL_TEXTURE_2D,materialTex,0);
    GLenum drawBuffers[] = {GL_NONE,GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3};
    glDrawBuffers(5,drawBuffers);

    // テクスチャ処理は一旦完了
    glBindTexture(GL_TEXTURE_2D,0);

    // 確認表示
    Logger::Log("g-Buffer locations " + std::to_string(positionLoc)
            + " " + std::to_string(normalLoc)  
            + " " + std::to_string(colorLoc)
            + " " + std::to_string(materialLoc)
            );

}

void Renderer::init(std::shared_ptr<ShaderManager> shaderManager){
    shader = shaderManager;

    makeBuffers();

    GLuint _shaderID = shader->programID;

    renderTarget.Init(_shaderID);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Renderer::setScene(std::shared_ptr<SceneManager> scene){
    this->scene = scene;
}

void Renderer::render(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // パス１
    glBindFramebuffer(GL_FRAMEBUFFER,deferredFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->useShader();
    glClearColor(0.1f,0.1f,0.2f,1.0f);
    shader->setPass(1); 
    glViewport(0,0,width,height);
    scene->drawScene(shader);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(width*2/3,height*2/3,width/3,height/3);
    scene->drawScene(shader);

    // パス２
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClearColor(0.2f,0.1f,0.1f,1.0f);
    shader->setPass(2);

    for(auto light : scene->lights){
        light.Set(scene->camera.getViewMatrix());
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,posTex);
    glUniform1i(positionLoc,0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,normTex);
    glUniform1i(normalLoc,1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,colorTex);
    glUniform1i(colorLoc,2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D,materialTex);
    glUniform1i(materialLoc,3);

    glViewport(0,0,width*2/3,height*2/3);
    renderTarget.Draw();

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

