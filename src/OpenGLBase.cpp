
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

#include "ShaderManager.h"
#include "SceneMaker.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Input.h"

using namespace std;


int main(int argc,char* argv[]){

    GLFWwindow* window;

    // GLFW初期化
    if(!glfwInit()) {
        cout << "glfwInit failure" << endl;
        return -1;
    }

    // OpenGLのバージョン設定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // glfwCreateWindow(width,height,title,monitor,share)
    //      monitor : 使用するモニタ　フルスクリーンでない場合にはNULLを指定
    //      share : 他のウィンドウとのリソースの共有に使う　windowのハンドルを指定
    window = glfwCreateWindow(1280,800, "Sample",NULL,NULL);
    if(!window){
        cout << "window open error" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // GLEWの初期化
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        cout << "Glew Init Failulre" << endl;
        return 0;
    }

    // この時点でのOpenGLバージョン確認
    cout << "GPU:" << glGetString(GL_VERSION) << endl
        << "OpenGL version : " << glGetString( GL_VERSION) << endl
        << "GLSL version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // シェーダーの準備
    ShaderManager shaderManager;
    shaderManager.makeShader("VertexShader.glsl","FragmentShader.glsl");
    GLuint programID = shaderManager.programID[0];

    ResourceManager resourceManager(programID);
    resourceManager.makeResource();

    SceneMaker sceneMaker(resourceManager);
    SceneManager myScene = sceneMaker.makeScene();

    myScene.initScene(programID);
    resourceManager.initResources(programID);


    // キー入力コールバックの登録
    Input* input = Input::createInstance(window);

    cout << "Draw Start" << endl;

    // 時間測定準備
    std::chrono::time_point<std::chrono::system_clock> start,end;
    int counter = 0;

    // メインループ
    while(!glfwWindowShouldClose(window)){
        start = std::chrono::system_clock::now();

        myScene.drawScene(programID);

        
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



