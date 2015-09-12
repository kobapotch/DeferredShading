
#include "Logger.h"
#include "RenderDriver.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

RenderDriver::RenderDriver(GLFWwindow* w) : renderer(720*2,480*2){
    window = w;
}

void RenderDriver::run(){

    // シェーダーの作成
    Logger::Log("--Make Shader");
    shaderManager = std::make_shared<ShaderManager>();
    shaderManager->makeShader("VertexShader.glsl","FragmentShader.glsl");
    GLuint programID = shaderManager->programID;

    // マテリアルの作成
    Logger::Log("--Make Material");
    resourceManager = std::make_shared<ResourceManager>();
    resourceManager->makeResource(programID);

    // シーンの作成
    Logger::Log("--Make Scene");
    SceneMaker sceneMaker(resourceManager);
    sceneManager = sceneMaker.makeScene();

    // 描画準備
    Logger::Log("--Prepare draw");
    resourceManager->initResources(programID);
    sceneManager->initScene(programID);
    renderer.init(shaderManager);
    renderer.setScene(sceneManager);

    sceneManager->printScene();


    // 描画ループ
    // 時間測定準備
    std::chrono::time_point<std::chrono::system_clock> start,end;
    int counter = 0;

    // メインループ
    Logger::Log("--Draw Start");
    while(!glfwWindowShouldClose(window)){
        start = std::chrono::system_clock::now();

        renderer.render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        // glfwWaitEvents();
        
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        if(counter%100==0){
            cout << "FPS : " <<  1 / elapsed_seconds.count() << endl;
            counter=0;
        }
        counter++;
        
    }
    glfwTerminate();
}


