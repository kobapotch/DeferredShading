#ifndef _TRANSFORM_INCLUDED
#define _TRANSFORM_INCLUDED

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

class Transform{
    public:
        glm::vec3 scale;
        glm::vec3 position;
        glm::vec3 rotation;


        Transform(){
            scale = glm::vec3(1,1,1);
            position = glm::vec3(0,0,0);
            rotation = glm::vec3(0,0,0);
        }

        float angle = 0;

        glm::mat4 getModelMatrix(){
            /* glm::mat4 model = glm::mat4(1.0f);
            angle += 0.02f;
            model = glm::rotate(
                    glm::mat4(1.0f),
                    angle,
                    glm::vec3(0.0f,1.0f,0.0f)
                    );
            */
            float toRad = M_PI / 180.0;
            glm::mat4 model = glm::translate(position) * (glm::eulerAngleYXZ(rotation.y*toRad,rotation.x*toRad,rotation.z*toRad) * glm::scale(scale));
            
            return model;
        }


};

#endif
