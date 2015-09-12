#include "ShaderManager.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "Logger.h"

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

    programID = ID;
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

    // uniform変数の書き出し
    GLint num,maxLen;
    glGetProgramiv(programID,GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxLen);
    glGetProgramiv(programID,GL_ACTIVE_UNIFORMS,&num);
    Logger::Log("Program Info : " + std::to_string(programID) + ","+ std::to_string(num));

    GLchar* name = (GLchar*) malloc(maxLen);

    GLint size,location;
    GLsizei written;
    GLenum type;
    Logger::Log(" Location | Name");
    for(int i=0;i<num;++i){
        glGetActiveUniform(programID,i,maxLen,&written,&size,&type,name);
        location = glGetUniformLocation(programID,name);
        Logger::Log(std::to_string(location) + " | " + std::string(name));
        ubo_dict[std::string(name)] = location;
    }
    free(name);

    vertexPass[0] = glGetSubroutineIndex(programID,GL_VERTEX_SHADER,"pass1");
    vertexPass[1] = glGetSubroutineIndex(programID,GL_VERTEX_SHADER,"pass2");
    fragmentPass[0] = glGetSubroutineIndex(programID,GL_FRAGMENT_SHADER,"pass1");
    fragmentPass[1] = glGetSubroutineIndex(programID,GL_FRAGMENT_SHADER,"pass2");

    Logger::Log("subroutine index " + std::to_string(vertexPass[0]) + " " + std::to_string(vertexPass[1])
            + " " + std::to_string(fragmentPass[0]) + " " + std::to_string(fragmentPass[1]));



}

void ShaderManager::useShader(){
    glUseProgram(programID);
}

void ShaderManager::setPass(int passNum){
    glUniformSubroutinesuiv(GL_VERTEX_SHADER,1,&(vertexPass[passNum-1]));
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER,1,&(fragmentPass[passNum-1]));

}

void ShaderManager::checkDict(std::string name){
    // ubo辞書にないuniformの書き込みがある時は，indexを探してから
    if(ubo_dict.find(name) == ubo_dict.end()){
        GLuint index = glGetUniformLocation(programID,name.c_str());
        ubo_dict[name] = index;
        Logger::Log("--Warning : ubo_dict not found of " + std::string(name));
    }
}

void ShaderManager::setUniform(std::string name, const glm::vec3& v){
    checkDict(name);
    glUniform3fv(ubo_dict[name],1,&v[0]);
}
void ShaderManager::setUniform(std::string name, const glm::vec4& v){
    checkDict(name);
    glUniform4fv(ubo_dict[name],1,&v[0]);
}
void ShaderManager::setUniform(std::string name, const glm::mat3& m){
    checkDict(name);
    glUniformMatrix3fv(ubo_dict[name],1,GL_FALSE,&m[0][0]);
}
void ShaderManager::setUniform(std::string name, const glm::mat4& m){
    checkDict(name);
    glUniformMatrix4fv(ubo_dict[name],1,GL_FALSE,&m[0][0]);
}
void ShaderManager::setUniform(std::string name,const float val){
    checkDict(name);
    glUniform1f(ubo_dict[name],val);
}
void ShaderManager::setUniform(std::string name, const int val){
    checkDict(name);
    glUniform1i(ubo_dict[name],val);
}




