#include "RenderTarget.h"

GLfloat pos[] = {
    1,1,0,
    -1,1,0,
    -1,-1,0,
    1,-1,0
};
GLfloat uv[] = {
    1,1,
    0,1,
    0,0,
    1,0
};
GLfloat normal[] = {
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1
};

RenderTarget::RenderTarget() : Primitive(){
    for(int i=0;i<4*3;++i){
        positionData.push_back(pos[i]);
        colorData.push_back(0);
        normalData.push_back(normal[i]);
    }
    for(int i=0;i<4*2;++i){
        uvData.push_back(uv[i]);
    }
}

void RenderTarget::DrawVertex(){
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
}


