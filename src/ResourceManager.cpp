
#include "ResourceManager.h"

void ResourceManager::makeResource(){

    Logger::Log("Make Resources");

    loadTexture("renga02.jpg",0);
    loadTexture("garasubo.bmp",0);

    std::shared_ptr<CubeMap> cMap(new CubeMap);
    cMap->loadCubeTex("right.png","left.png",
            "up.png","down.png",
            "front.png","back.png"
            );
    cubeMap = cMap;

    std::shared_ptr<Material> mat(new Material);
    materials.push_back(mat);
    
    std::shared_ptr<Material> mat2(new Material);
    mat2->Ka = glm::vec3(0.5,0.5,0.5);
    mat2->Kd = glm::vec3(1.0,1.0,1.0);
    mat2->Ks = glm::vec3(0.1,0.1,0.1);
    mat2->waterness = 0.8;
    materials.push_back(mat2);

    std::shared_ptr<Material> mat3(new Material);
    mat3->waterness = 1;
    materials.push_back(mat3);

}
