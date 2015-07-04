
#include <cmath>

#include "Animation.h"
#include "Input.h"

Animation::Animation(Transform* trans){
    transform = trans;
}

void RotateAnimation::Animate(){
    transform->rotation.y += dAngle;
}

void CircleTurningAnimation::Animate(){
    float x = rad * cos(angle);
    float y = transform->position.y;
    float z = rad * sin(angle);

    transform->position = glm::vec3(x,y,z);

    angle += dAngle;
}

void ArrowKeyAnimation::Animate(){
    KeyDirection kd = input->getKeyDirection();

    if(kd == UP){theta += dTheta;}
    if(kd == DOWN){ theta -= dTheta;}
    if(kd == LEFT) { phi -= dPhi;}
    if(kd == RIGHT) { phi += dPhi;}

    float x = rad*sin(theta)*cos(phi);
    float z = rad*sin(theta)*sin(phi);
    float y = rad*cos(theta);

    transform->position = glm::vec3(x,y,z);


}
