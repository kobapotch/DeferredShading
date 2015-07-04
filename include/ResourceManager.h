#ifndef _KOBA_RESOURCE_MANAGER_INCLUDED
#define _KOBA_RESOURCE_MANAGER_INCLUDED

#include <vector>
#include <memory>

#include "Material.h"
#include "Texture.h"
#include "Logger.h"
#include "CubeMap.h"

class ResourceManager{
    int shaderID;
    
    public:

        std::vector<std::shared_ptr<Material> > materials;
        std::vector<std::shared_ptr<Texture> > textures;
        std::shared_ptr<CubeMap> cubeMap;

        ResourceManager(int shaderID){
            this->shaderID = shaderID;
        }

        void loadTexture(const char* filename,int unit){
            std::shared_ptr<Texture> texture(new Texture);
            texture->loadTexture(filename);
            texture->makeTexture(shaderID,unit);            
            textures.push_back(texture);
        }

        void makeResource();

        void initResources(GLuint shaderID){
            for(auto mat : materials){
                mat->Init(shaderID);
            }
            cubeMap->makeCubeMap(shaderID,2);

        }

};

#endif
