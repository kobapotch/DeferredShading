#include "ShaderManager.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

// #include "Logger.h"

using namespace std;


void ShaderManager::compileShader(const char* file_path,GLuint &shaderID){
    // シェーダ文字列の読み込み  
    string shaderCode;
    ifstream shaderStream(file_path,ios::in);
    if(shaderStream.is_open()){
        string line = "";
        while(getline(shaderStream,line)){
            shaderCode += line + "\n";
        }
        shaderStream.close();
    }
    
    int result = GL_FALSE;
    int infoLogLength;

    // シェーダーのコンパイル
    cout << "Compiling shader : " << file_path << endl;
    char const *sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID,1,&sourcePointer,NULL);
    glCompileShader(shaderID);

    // シェーダーのチェック
    glGetShaderiv(shaderID,GL_COMPILE_STATUS,&result);
    glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&infoLogLength);
    vector<char> shaderErrorMessage(infoLogLength);
    glGetShaderInfoLog(shaderID,infoLogLength,NULL,&shaderErrorMessage[0]);

    for(int i=0;i<shaderErrorMessage.size();i++){
        cout << shaderErrorMessage[i];
    }cout << endl;

}

void ShaderManager::linkShader(std::vector<GLuint> shaderID){
    // プログラムのリンク
    cout << "Linking program" << endl;
    for(int i=0;i<shaderID.size();++i){
        cout << " " << shaderID[i];
    }cout << endl;

    GLuint ID = glCreateProgram();
    for(int i=0;i<shaderID.size();++i){
        glAttachShader(ID,shaderID[i]);
    }
    glLinkProgram(ID);

    // プログラムのチェック
    int result;
    int infoLogLength;
    // プログラムのチェック
    glGetProgramiv(ID,GL_LINK_STATUS,&result);
    glGetProgramiv(ID,GL_INFO_LOG_LENGTH,&infoLogLength);
    vector<char> ProgramErrorMessage( max(infoLogLength,int(1)));
    glGetProgramInfoLog(ID,infoLogLength,NULL,&ProgramErrorMessage[0]);
    if(result == GL_FALSE){std::cout << "Link failure" << result<< std::endl;}
    for(int i=0;i<ProgramErrorMessage.size();i++){
        cout << ProgramErrorMessage[i];
    }cout << endl;

    for(int i=0;i<shaderID.size();++i){
    glDeleteShader(shaderID[i]);
    }
    // Log("Linked program.");
    cout << "Linked program : " << ID << endl;

    programID.push_back(ID);
}

void ShaderManager::makeShader(const char* vertexShader,const char* fragmentShader){
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    compileShader(vertexShader,vertexShaderID);

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(fragmentShader,fragmentShaderID);

    std::vector<GLuint> shaderID;
    shaderID.push_back(vertexShaderID);
    shaderID.push_back(fragmentShaderID);

    linkShader(shaderID);

}

