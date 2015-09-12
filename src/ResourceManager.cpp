
#include "ResourceManager.h"

void ResourceManager::loadTexture(const char* filename,int unit){
    std::shared_ptr<Texture> texture(new Texture);
    texture->loadTexture(filename,unit);
    textures.push_back(texture);
}

void ResourceManager::makeResource(GLuint shaderID){

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
    mat->Roughness = 0.1f;
    mat->Metallic = 0.8f;
    materials.push_back(mat);
    
    std::shared_ptr<Material> mat2(new Material);
    mat2->Roughness = 0.5f;
    mat2->Metallic = 0.8f;
    materials.push_back(mat2);

    std::shared_ptr<Material> mat3(new Material);
    mat3->Roughness = 0.9f;
    mat3->Metallic = 0.8f;
    materials.push_back(mat3);

    std::shared_ptr<Material> matb(new Material);
    matb->Roughness = 0.1f;
    matb->Metallic = 0.2f;
    materials.push_back(matb);

    std::shared_ptr<Material> mat2b(new Material);
    mat2b->Roughness = 0.5f;
    mat2b->Metallic = 0.2f;
    materials.push_back(mat2b);

    std::shared_ptr<Material> mat3b(new Material);
    mat3b->Roughness = 0.9f;
    mat3b->Metallic = 0.2f;
    materials.push_back(mat3b);

}
