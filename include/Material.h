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

        float Metallic;
        float Roughness;
        float F0;

        Material(){
            Metallic = 1.0f;
            Roughness = 0.5f;
            F0 = 0.8f;
        }

        // シェーダー内のuniform blockにデータをバインド 
        void Init(GLuint shaderID){

            Logger::Log("Material Init");        

            string uniformName = string("Material"); 
            
            cout << "shaderID " << shaderID << endl;

            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".Metallic").c_str() ));
            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".Roughness").c_str() ));
            ubo.push_back(glGetUniformLocation( shaderID,(uniformName + ".F0").c_str() ));

            cout << "material ubo :";
            for(auto i : ubo){
                cout << " " << i;
            }cout << endl;

        }

        void Set(){
            glUniform1f(ubo[0],Metallic);
            glUniform1f(ubo[1],Roughness);
            glUniform1f(ubo[2],F0);
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
