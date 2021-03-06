// HelloTriangle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <math.h>
#include "r_funcs.h"

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <sstream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

void framebuffer_size_cb(GLFWwindow* window, int width, int height);

int main()
{
  if (!glfwInit())
    return utility::r_exit(-1, "Cannot initiate opengl.");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);*/

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Texture", NULL, NULL);

  if (window == NULL)
    return utility::r_exit(-1);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);


  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);

  std::stringstream inf;
  inf << "Renderer:" << renderer << std::endl << "OpenGL version supported:" << version << std::endl;
  utility::dbg_print(inf.str());



  int result = r_draw_light(window);

  return utility::r_exit(result);
}



void framebuffer_size_cb(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
