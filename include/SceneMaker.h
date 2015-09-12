#ifndef _KOBA_SCENEMAKER_INCLUDED
#define _KOBA_SCENEMAKER_INCLUDED

#include <memory>

#include "fbxsdk.h"
#include "SceneManager.h"
#include "ResourceManager.h"

class SceneMaker{
    private:
        std::shared_ptr<ResourceManager> resourceManager; 
        std::shared_ptr<Primitive> meshToPrimitive(FbxMesh*);
        void RecursiveMakeMesh(FbxNode*,SceneManager&);
        void FbxToMyScene(FbxScene*,SceneManager&);

    public:

        SceneMaker(std::shared_ptr<ResourceManager>);
        std::unique_ptr<SceneManager> makeScene();    

        
};

#endif
