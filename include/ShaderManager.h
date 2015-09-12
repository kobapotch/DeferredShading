#ifndef _SHADERMANAGER_INCLUDED
#define _SHADERMANAGER_INCLUDED

#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class ShaderManager{
    private:

        void compileShader(const char*, GLuint&);
        void linkShader(std::vector<GLuint> shaderID);

        // UniformBufferObject
        std::map<std::string,GLuint> ubo_dict;
        // サブルーチンのインデックス
        GLuint fragmentPass[2], vertexPass[2];

        void checkDict(std::string);

    public:

        // リンクしたシェーダーのID
        GLuint programID;
        void makeShader(const char* vertexShader,const char* FragmentShader);

        void useShader();

        void setPass(int passNum);

        void setUniform(std::string name, const glm::vec3&);
        void setUniform(std::string name, const glm::vec4&);
        void setUniform(std::string name, const glm::mat3&);
        void setUniform(std::string name, const glm::mat4&);
        void setUniform(std::string name, const float);
        void setUniform(std::string name, const int);

};

#endif


