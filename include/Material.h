#ifndef _KOBA_MATERIAL_INCLUDED
#define _KOBA_MATERIAL_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Logger.h"

using namespace std;

class Material{
    private:
        std::vector<GLuint> ubo; 

    public:

        glm::vec3 Ka;
        glm::vec3 Kd;
        glm::vec3 Ks;
        float shininess;
        float waterness;

        Material(){
            Ka = glm::vec3(0.1,0.1,0.1);
            Kd = glm::vec3(0.5,0.5,0.5);
            Ks = glm::vec3(0.5,0.5,0.5);
            shininess = 20;
            waterness = 0;
        }

        // シェーダー内のuniform blockにデータをバインド 
        void Init(GLuint shaderID){

            Logger::Log("Material Init");        

            string uniformName = string("Material"); 

            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".Ka").c_str() ));
            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".Kd").c_str() ));
            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".Ks").c_str() ));
            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".shininess").c_str() ));
            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".waterness").c_str() ));

            cout << "material ubo :";
            for(auto i : ubo){
                cout << " " << i;
            }cout << endl;

        }

        void Set(){
            glUniform3fv(ubo[0],1,&Ka[0]);
            glUniform3fv(ubo[1],1,&Kd[0]);
            glUniform3fv(ubo[2],1,&Ks[0]);

            glUniform1f(ubo[3],shininess);
            glUniform1f(ubo[4],waterness);
        }
};

class MaterialManager{
    public:
        std::vector<Material> materials;
        void initMaterials(GLuint shaderID){
            for(auto &m : materials){
                m.Init(shaderID);
            }
        }
};


#endif
