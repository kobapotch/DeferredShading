#ifndef _SHADERMANAGER_INCLUDED
#define _SHADERMANAGER_INCLUDED

#include <vector>

#include <GLFW/glfw3.h>

class ShaderManager{
    private:

        void compileShader(const char*, GLuint&);
        void linkShader(std::vector<GLuint> shaderID);

    public:

        // リンクしたシェーダーのID
        std::vector<GLuint> programID;
        void makeShader(const char* vertexShader,const char* FragmentShader);
};

#endif


