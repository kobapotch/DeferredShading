#ifndef _KOBA_CAMERA_INCLUDED
#define _KOBA_CAMERA_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

#include "Transform.h"
#include "Animation.h"

class Camera{
    private:
        float aspect;
        float front;
        float back;

        // 水面の高さ
        float waterSurf = -10;
        bool invert = false;

    public:
        Transform transform;
        Animation* animation;

        float angle;

        Camera(float angle){
            this->angle = angle;
            aspect = 1.0f;
            front = 0.1f;
            back = 1000.0f;

            // animation = new CircleTurningAnimation(&transform);
            animation = new ArrowKeyAnimation(&transform);
        }
        ~Camera(){
            if(animation != NULL) delete animation;
        }

        glm::mat4 getProjectionMatrix(){
            glm::mat4 projection = glm::perspective(
                    angle,aspect,front,back);
            return projection;

        }

        float rotAngle=0;
        glm::mat4 getViewMatrix(){

            glm::vec3 pos = transform.position;
            if(invert) pos.y = -(pos.y-waterSurf) + waterSurf;

            
            glm::mat4 view = glm::lookAt(
                    pos,
                    glm::vec3(0,0,0),
                    glm::vec3(0,1,0)
                    );
            if(invert) view = glm::lookAt(
                    pos,
                    glm::vec3(0,waterSurf*2,0),
                    glm::vec3(0,1,0)
                    );


            return view;
        }
        glm::vec3 getPosition(){
            glm::vec3 pos = transform.position;
            if(invert) pos.y = -(pos.y - waterSurf) + waterSurf;
            return pos;
        }

        void setInvert(bool inv){
            invert = inv;
        }


};

#endif
