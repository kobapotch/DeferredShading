#ifndef _RENDERDRIVER_INCLUDED
#define _RENDERDRIVER_INCLUDED

#include "ShaderManager.h"
#include "ResourceManager.h"
#include "SceneMaker.h"
#include "SceneManager.h"
#include "Renderer.h"

#include <memory>


class RenderDriver{
    private:
        GLFWwindow* window;

        std::shared_ptr<ShaderManager> shaderManager;
        std::shared_ptr<ResourceManager> resourceManager;
        std::shared_ptr<SceneManager> sceneManager;
        Renderer renderer;

    public:
        RenderDriver(GLFWwindow*);
        void run();

};

#endif
