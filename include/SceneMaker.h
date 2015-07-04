#ifndef _KOBA_SCENEMAKER_INCLUDED
#define _KOBA_SCENEMAKER_INCLUDED

#include <memory>

#include "fbxsdk.h"
#include "SceneManager.h"
#include "ResourceManager.h"

class SceneMaker{
    private:
        ResourceManager* resourceManager; 
        std::shared_ptr<Primitive> meshToPrimitive(FbxMesh*,Camera&);
        void RecursiveMakeMesh(FbxNode*,SceneManager&);
        void FbxToMyScene(FbxScene*,SceneManager&);

    public:

        SceneMaker(ResourceManager&);
        SceneManager makeScene();    

        
};

#endif
