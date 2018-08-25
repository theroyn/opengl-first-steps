// HelloTriangle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <sstream>

using namespace std;

int r_exit(int, const string &msg = "");
int r_draw_triangle(GLFWwindow *);

int main()
{
  if (!glfwInit())
    return r_exit(-1, "Cannot initiate opengl.");

  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if (!window)
  { 
    return r_exit(-1);
  }

  glfwMakeContextCurrent(window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);

  std::stringstream inf;
  inf << "Renderer:" << renderer << endl << "OpenGL version supported:" << version << endl;
  dbg_print(inf.str());

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  int result = r_draw_triangle(window);

  return r_exit(result);
}

int r_draw_triangle(GLFWwindow *window)
{
  GLfloat sqr_pnts[] =
  {
    -0.5f,  0.5f,  0.0f,
    0.5f, 0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
    0.5f, -0.5f,  0.0f
  };

  GLfloat ur_pnts[] =
  {
    0.75f, 1.f, 0.f,
    1.f, 0.75f, 0.f,
    0.75f, 0.75f, 0.f
  };

  GLuint vbo[2] = { 0 };
  glGenBuffers(2, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqr_pnts, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), ur_pnts, GL_STATIC_DRAW);


  GLuint vao1 = 0;
  glGenVertexArrays(1, &vao1);
  glBindVertexArray(vao1);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint vao2 = 0;
  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLint result = GL_TRUE;
  vector<GLchar> msg;
  GLuint shader_programme = load_program("vertex_shader.glsl", "fragment_shader.glsl", result, msg);
  GLuint yellow_shader_programme = load_program("vertex_shader.glsl", "yellow_fragment_shader.glsl", result, msg);

  if (result == GL_FALSE)
  {
    dbg_print(&msg[0]);

    return -1;
  }

  //glClearColor(0.f, 0.9f, 0.4f, 1.f);

  while (!glfwWindowShouldClose(window))
  {
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_programme);
    
    glBindVertexArray(vao1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUseProgram(yellow_shader_programme);
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    //glPointSize(10.f);
    //glDrawArrays(GL_LINES, 0, 2);
    //glDrawArrays(GL_LINES, 1, 3);
    //glDrawArrays(GL_TRIANGLES, 1, 4);

    // update other events like input handling 
    glfwPollEvents();

    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
  }

  return 0;
}


