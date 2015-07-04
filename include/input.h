#ifndef _KOBA_INPUT_INCLUDED
#define _KOBA_INPUT_INCLUDED

#include <GLFW/glfw3.h>

enum KeyDirection{
    UP=0,DOWN,LEFT,RIGHT,NONE
};

class Input{
    private:
        Input(){};

        static Input* instance;
        static GLFWwindow* window;
    public:
        static Input* createInstance(GLFWwindow* _window){
            window = _window;
            return getInstance();
        }

        static Input* getInstance(){
            if(instance == NULL){
                instance = new Input();
            }
            return instance;
            
        }

        KeyDirection getKeyDirection();


};


#endif
