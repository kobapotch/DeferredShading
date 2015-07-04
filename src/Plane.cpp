#include "Primitive.h"

GLfloat planePos[] = {
    -0.5,0.5,0.0,
    -0.5,-0.5,0.0,
    0.5,-0.5,0.0,
    0.5,0.5,0.0,
};
GLfloat planeuv[] = {
    0,1,
    0,0,
    1,0,
    1,1,

};
GLfloat planeNor[] = {
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1
};

Plane::Plane(Camera* camera) : Primitive(camera){
    for(int i=0;i<4*3;i++){
        positionData.push_back(planePos[i]);
        colorData.push_back(0);
        normalData.push_back(planeNor[i]);
    }for(int i=0;i<4*2;i++){
        uvData.push_back(planeuv[i]);
    }   

}

void Plane::DrawVertex(){
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
}
