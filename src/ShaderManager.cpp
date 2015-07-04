#include "ShaderManager.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

// #include "Logger.h"

using namespace std;

void ShaderManager::compileVertexShader(const char* file_path){
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    compileShader(file_path,vertexShaderID);
}

void ShaderManager::compileFragmentShader(const char* file_path){
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(file_path,fragmentShaderID);
}

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

GLuint ShaderManager::linkShader(){
    // プログラムのリンク
    cout << "Linking program" << endl;
    cout << vertexShaderID << " : " << fragmentShaderID << endl;

    programID = glCreateProgram();
    glAttachShader(programID,vertexShaderID);
    glAttachShader(programID,fragmentShaderID);
    glLinkProgram(programID);

    // プログラムのチェック
    int result;
    int infoLogLength;
    // プログラムのチェック
    glGetProgramiv(programID,GL_LINK_STATUS,&result);
    glGetProgramiv(programID,GL_INFO_LOG_LENGTH,&infoLogLength);
    vector<char> ProgramErrorMessage( max(infoLogLength,int(1)));
    glGetProgramInfoLog(programID,infoLogLength,NULL,&ProgramErrorMessage[0]);
    if(result == GL_FALSE){std::cout << "Link failure" << result<< std::endl;}
    for(int i=0;i<ProgramErrorMessage.size();i++){
        cout << ProgramErrorMessage[i];
    }cout << endl;

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    // Log("Linked program.");
    cout << "Linked program : " << programID << endl;

    return programID;
}
