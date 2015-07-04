#ifndef _SHADERMANAGER_INCLUDED
#define _SHADERMANAGER_INCLUDED

#include <GLFW/glfw3.h>

class ShaderManager{
private:
    
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    // リンクしたシェーダーのID
    GLuint programID;

    void compileShader(const char*, GLuint&);

public:

    void compileVertexShader(const char*);
    void compileFragmentShader(const char*);

    GLuint linkShader();
};

#endif


