#include "pch.h"
#include "r_funcs.h"
#include <algorithm>

int r_draw_texture_box(GLFWwindow *window)
{
  GLfloat sqr_pnts[] = /** (r,g,b,u,v) */
  {
     0.5f,  0.5f,  0.f, 1.f, 1.f, /** TR */
     0.5f, -0.5f,  0.f, 1.f, 0.f, /** BR */
    -0.5f, -0.5f,  0.f, 0.f, 0.f, /** BL */
    -0.5f,  0.5f,  0.f, 0.f, 1.f  /** TL */
  };

  GLuint sqr_indices[] =
  {
    0, 1, 2,
    0, 2, 3
  };

  GLfloat visibility_val = 0.2f, speed = 0.0002f;

  GLuint vao[1] = { 0 };
  glGenVertexArrays(1, vao);
  glBindVertexArray(vao[0]);

  GLuint vbo[2] = { 0 };
  glGenBuffers(2, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(sqr_pnts), sqr_pnts, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sqr_indices), sqr_indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0));

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  GLint result = GL_TRUE;
  vector<GLchar> msg;

  GLuint texture1 = load_texture("container.jpg", result, msg, false);
  GLuint texture2 = load_texture("awesomeface.png", result, msg, true);

  r_handle_result(result, msg);
    
  GLuint shader_programme = load_program("texture_box_vs.glsl", "texture_box_fs.glsl", result, msg);

  r_handle_result(result, msg);

  glUseProgram(shader_programme);
  GLuint visibility_loc = glGetUniformLocation(shader_programme, "vis");
  glUniform1f(visibility_loc, visibility_val);
  glUniform1i(glGetUniformLocation(shader_programme, "texture_sampler1"), 0);
  glUniform1i(glGetUniformLocation(shader_programme, "texture_sampler2"), 1);

  while (!glfwWindowShouldClose(window))
  {
    r_update_fps_counter(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glUseProgram(shader_programme);
    glBindVertexArray(vao[0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE) ||
      GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ENTER))
      glfwSetWindowShouldClose(window, GL_TRUE);

    if (int key_u = glfwGetKey(window, GLFW_KEY_UP),
        key_d = glfwGetKey(window, GLFW_KEY_DOWN);
          GLFW_PRESS == key_u ||  GLFW_PRESS == key_d)
    {
      int dir = key_u ? 1 : -1;
      visibility_val += (speed * dir);
      visibility_val = clamp(visibility_val, 0.f, 1.f);
      glUniform1f(visibility_loc, visibility_val);
    }


  }

  return R_SUCCESS;
}

int r_draw_triangle(GLFWwindow *window)
{
  GLfloat ur_pnts[] =
  {
    0.75f, 1.f, 0.f,
    1.f, 0.75f, 0.f,
    0.75f, 0.75f, 0.f
  };

  GLfloat sqr_pnts[] =
  {
    -0.5f,  0.5f,  0.f,
    0.5f, 0.5f,  0.f,
    -0.5f, -0.5f,  0.f,
    0.5f, -0.5f,  0.f,
    0.5f, -1.f,  0.f
  };

  GLfloat pnts_colours[] =
  {
    1.f,  0.f,  0.f,
    0.f, 1.f,  0.f,
    0.f, 0.f,  1.f,
    1.f, 1.f,  1.f,
    0.f, 1.f,  1.f
  };

  GLuint vbo[4] = { 0 };
  glGenBuffers(4, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), sqr_pnts, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), pnts_colours, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), ur_pnts, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), pnts_colours, GL_STATIC_DRAW);


  GLuint vao[2] = { 0 };
  glGenVertexArrays(2, vao);
  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glBindVertexArray(vao[1]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLint result = GL_TRUE;
  vector<GLchar> msg;
  GLuint shader_programme = load_program("vertex_shader.glsl", "fragment_shader.glsl", result, msg);

  if (result == GL_FALSE)
  {
    dbg_print(&msg[0]);

    return R_FAILURE;
  }
  GLuint shader_programme2 = load_program("vertex_shader.glsl", "fragment_shader.glsl", result, msg);

  if (result == GL_FALSE)
  {
    dbg_print(&msg[0]);

    return R_FAILURE;
  }

  //glClearColor(0.f, 0.9f, 0.4f, 1.f);
  /**glEnable(GL_CULL_FACE); // cull face
  glCullFace(GL_BACK); // cull back face
  glFrontFace(GL_CW); // GL_CCW for counter clock-wise
  */
  bool is_sqr = true;
  glBindVertexArray(vao[0]);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(vao[1]);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  GLuint light_location = glGetUniformLocation(shader_programme, "light");

  while (!glfwWindowShouldClose(window))
  {
    r_update_fps_counter(window);
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_programme);

    float light = (sin(glfwGetTime()) + 1) * 0.5f;
    glUniform1f(light_location, light);

    if (is_sqr)
    {
      glBindVertexArray(vao[0]);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
    }
    else
    {
      glBindVertexArray(vao[1]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }



    // update other events like input handling 
    glfwPollEvents();

    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE) ||
      GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ENTER))
      glfwSetWindowShouldClose(window, GL_TRUE);

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE))
      is_sqr = !is_sqr;
  }

  return R_SUCCESS;
}