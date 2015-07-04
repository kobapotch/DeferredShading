
#include "Primitive.h"
#include "Logger.h"


GLfloat g_vertex_buffer_data[] ={
    // 前面
    -0.5f,0.5f,0.5f,
    -0.5f,-0.5f,0.5f,
    0.5f,-0.5f,0.5f,
    0.5f,0.5f,0.5f,
    // 上面
    -0.5f,0.5f,-0.5f,
    -0.5f,0.5f,0.5f,
    0.5f,0.5f,0.5f,
    0.5f,0.5f,-0.5f,
    // 左面
    -0.5f,0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,0.5f,
    -0.5f,0.5f,0.5f,
    // 下面
    -0.5f,-0.5f,0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,0.5f,
    // 右面
    0.5f,0.5f,0.5f,
    0.5f,-0.5f,0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,0.5f,-0.5f,
    // 裏面
    0.5f,0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,0.5f,-0.5f
};



GLfloat g_color_data[] = {
    // 前面
    0.8f,  0.5f,  0.2f,
    0.8f,  0.5f,  0.2f,
    0.8f,  0.5f,  0.2f,
    0.8f,  0.5f,  0.2f,
    // 上面
    0.5f,0.2f,0.8f,
    0.5f,0.2f,0.8f,
    0.5f,0.2f,0.8f,
    0.5f,0.2f,0.8f,
    // 左面
    0.2f,0.8f,0.5f,
    0.2f,0.8f,0.5f,
    0.2f,0.8f,0.5f,
    0.2f,0.8f,0.5f,
    // 下面
    0.8f,0.2f,0.5f,
    0.8f,0.2f,0.5f,
    0.8f,0.2f,0.5f,
    0.8f,0.2f,0.5f,
    // 右面
    0.2f,0.5f,0.8f,
    0.2f,0.5f,0.8f,
    0.2f,0.5f,0.8f,
    0.2f,0.5f,0.8f,
    // 下面
    0.5f,0.8f,0.2f,
    0.5f,0.8f,0.2f,
    0.5f,0.8f,0.2f,
    0.5f,0.8f,0.2f
};

GLfloat g_normal_data[] = {
    // 前面
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    // 上面
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    // 左面
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    // 下面
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    // 右面
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    // 奥面
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f
};

GLfloat g_uv_data[] ={
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f,
    // 全面
    0.0f,1.0f,
    0.0f,0.0f,
    1.0f,0.0f,
    1.0f,1.0f
};


Cube::Cube(Camera* camera) : Primitive(camera){

    for(int i=0;i<24*3;i++){
        positionData.push_back(g_vertex_buffer_data[i]);
        colorData.push_back( g_color_data[i]);
        normalData.push_back( g_normal_data[i]);
    }
    for(int i=0;i<24*2;i++){
        uvData.push_back(g_uv_data[i]);
    } 

}

void Cube::DrawVertex(){
    // 頂点0から初めて，3つの頂点を利用して
    for(int i=0;i<6;i++){
        glDrawArrays(GL_TRIANGLE_FAN,i*4,4);
    }
}
