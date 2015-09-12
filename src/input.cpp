#include <GLFW/glfw3.h>
#include "Logger.h"
#include "input.h"

Input* Input::instance = NULL;
GLFWwindow* Input::window = NULL;

KeyDirection Input::getKeyDirection(){

    if(glfwGetKey(window,GLFW_KEY_UP)){ return UP;}
    if(glfwGetKey(window,GLFW_KEY_DOWN)) {return DOWN;}
    if(glfwGetKey(window,GLFW_KEY_LEFT)) {return LEFT;}
    if(glfwGetKey(window,GLFW_KEY_RIGHT)) {return RIGHT;}

    return NONE;
}

bool Input::getZoomInButton(){
    return glfwGetKey(window,GLFW_KEY_X);
}
bool Input::getZoomOutButton(){
    return glfwGetKey(window,GLFW_KEY_C);
}


