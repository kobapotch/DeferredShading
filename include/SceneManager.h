#ifndef _KOBA_SCENEMANAGER_INCLUDED
#define _KOBA_SCENEMANAGER_INCLUDED

#include <vector>
#include <memory>

#include "ResourceManager.h"
#include "ShaderManager.h"
#include "Primitive.h"
#include "Camera.h"
#include "Light.h"
#include "Logger.h"



class SceneManager{
    private:
        GLuint fbo;
        GLuint fboDepth;
        GLuint renderTex;

        GLuint whiteTexHandle;

    public:

        std::vector< std::shared_ptr<Primitive> > primitives;
        std::vector<std::shared_ptr<Plane> > planes;
        std::shared_ptr<Primitive> water;
        std::vector<Light> lights;
        Camera camera;

        shared_ptr<CubeMap> cubeMap;

        SceneManager(); 

        void addLight(Light light);
        void initScene(GLuint &shaderID);
        void drawScene(std::shared_ptr<ShaderManager>);

        void printScene();
};

#endif
