#ifndef INPUT_HPP 
#define INPUT_HPP 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "global.hpp"
#include <reactphysics3d/reactphysics3d.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

#endif
