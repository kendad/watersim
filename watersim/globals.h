#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//window value created on intilization
extern GLFWwindow* window;

extern Camera* gCamera;

extern bool IS_MOUSE_CAPTURED;

//intialize GLFW for processing
extern void INIT();


//#######################################################################################################################
// CALLBACK FUNCTIONS
//#######################################################################################################################
//window resize callback function
extern void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//process user input
extern void processInput(GLFWwindow* window,float& deltaTime);
//mouse callback function
extern void mouse_callback(GLFWwindow* window, double xPos, double yPos);
//keyboard input callback function
extern void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//#######################################################################################################################