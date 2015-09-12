#ifndef _KOBA_RESOURCE_MANAGER_INCLUDED
#define _KOBA_RESOURCE_MANAGER_INCLUDED

#include <vector>
#include <memory>

#include "Material.h"
#include "Texture.h"
#include "Logger.h"
#include "CubeMap.h"

class ResourceManager{
    private:
        void loadTexture(const char*filename,int unit);
    
    public:

        std::vector<std::shared_ptr<Material> > materials;
        std::vector<std::shared_ptr<Texture> > textures;
        std::shared_ptr<CubeMap> cubeMap;

        ResourceManager(){}
        
        // ここでマテリアルの作成や使用するテクスチャを記述 
        void makeResource(GLuint shaderID);

        // シェーダに配置する
        void initResources(GLuint shaderID){
            for(auto mat : materials){
                mat->Init(shaderID);
            }
            for(auto tex : textures){
                tex->makeTexture(shaderID);
            }
            cubeMap->makeCubeMap(shaderID,4);

        }

};

#endif
