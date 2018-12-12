#include "pch.h"
#include "r_funcs.h"
#include <algorithm>
#include "coords.h"
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Camera.h"

void process_input(GLFWwindow *window);
static void update_visibility(GLFWwindow *window);
static void mouse_drag_cb(GLFWwindow *window, double xpos, double ypos);
static void mouse_scroll_cb(GLFWwindow *window, double xoff, double yoff);

namespace params
{
  static GLfloat visibility_val = 0.f, visibility_speed = 0.002f;
  static GLuint visibility_loc;
  static Camera *camera = NULL;
}

/** ========================================================================= */
int r_draw_light(GLFWwindow *window)
/** ========================================================================= */
{
  glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_drag_cb);
  glfwSetScrollCallback(window, mouse_scroll_cb);


  GLuint vbo[] = { 0 };
  glGenBuffers(sizeof(vbo) / sizeof(vbo[0]), vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_coords_w_normals), cube_coords_w_normals, GL_STATIC_DRAW);


  GLuint vao[] = { 0, 0 };
  glGenVertexArrays(sizeof(vao) / sizeof(vao[0]), vao);

  glBindVertexArray(vao[0]);

  // Cube vertices.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast< void * >(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast< void * >(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(vao[1]);

  // Lamp vertices.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast< void * >(0));
  glEnableVertexAttribArray(0);

  GLint result = GL_TRUE;
  vector<GLchar> msg;

  GLuint box_shader = load_program("light_box_vs.glsl", "light_box_fs.glsl", result, msg);

  r_handle_result(result, msg);

  GLuint lamp_shader = load_program("lamp_shader_vs.glsl", "lamp_shader_fs.glsl", result, msg);

  r_handle_result(result, msg);

  glUseProgram(box_shader);

  GLuint box_model_loc = glGetUniformLocation(box_shader, "model");
  GLuint box_view_loc = glGetUniformLocation(box_shader, "view");
  GLuint box_projection_loc = glGetUniformLocation(box_shader, "projection");
  GLuint object_colour_loc = glGetUniformLocation(box_shader, "object_colour");
  GLuint light_colour_loc = glGetUniformLocation(box_shader, "light_colour");
  GLuint light_pos_loc = glGetUniformLocation(box_shader, "light_pos");
  GLuint light_view_pos_loc = glGetUniformLocation(box_shader, "light_view_pos");

  glUseProgram(lamp_shader);

  GLuint lamp_model_loc = glGetUniformLocation(lamp_shader, "model");
  GLuint lamp_view_loc = glGetUniformLocation(lamp_shader, "view");
  GLuint lamp_projection_loc = glGetUniformLocation(lamp_shader, "projection");
  glm::vec3 camera_pos(0.f, 0.f, 5.f);
  glm::vec3 camera_front(0.f, 0.f, -1.f);
  glm::vec3 world_up(0.f, 1.f, 0.f);
  params::camera = new Camera(window, camera_pos, camera_front, world_up);

  glm::vec3 lamp_pos(-1.f, 0.f, 1.f);
  glm::vec3 light_colour(1.f);
  glm::vec3 object_colour(1.f, .5f, .31f);


  while (!glfwWindowShouldClose(window))
  {
    r_update_fps_counter(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view_trans, projection_trans;
    glm::vec3 circle_delt(0.f);
    double p = glfwGetTime();
    circle_delt.x = 0.001*sin(p * 2);
    circle_delt.y = 0.001*sin(p * 2.5f);
    circle_delt.z = 0.001*cos(p * 2);

    view_trans = params::camera->get_view();
    projection_trans = params::camera->get_projection();

    // Draw box.
    glUseProgram(box_shader);
    glBindVertexArray(vao[0]);

    lamp_pos += circle_delt;
    glUniform3fv(light_pos_loc, 1, glm::value_ptr(lamp_pos));
    glUniform3fv(light_view_pos_loc, 1, glm::value_ptr(view_trans * glm::vec4(lamp_pos, 1.f)));
    glUniformMatrix4fv(box_view_loc, 1, GL_FALSE, glm::value_ptr(view_trans));
    glUniformMatrix4fv(box_projection_loc, 1, GL_FALSE, glm::value_ptr(projection_trans));

    glm::mat4 model_trans = glm::translate(glm::mat4(), cube_positions[0]);

    glUniformMatrix4fv(box_model_loc, 1, GL_FALSE, glm::value_ptr(model_trans));

    glUniform3fv(object_colour_loc, 1, glm::value_ptr(object_colour));
    glUniform3fv(light_colour_loc, 1, glm::value_ptr(light_colour));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw lamp.
    glUseProgram(lamp_shader);
    glBindVertexArray(vao[1]);

    //glUniform1f(lamp_alpha_loc, alpha);
    glUniformMatrix4fv(lamp_view_loc, 1, GL_FALSE, glm::value_ptr(view_trans));
    glUniformMatrix4fv(lamp_projection_loc, 1, GL_FALSE, glm::value_ptr(projection_trans));

    model_trans = glm::translate(glm::mat4(), lamp_pos);
    model_trans = glm::scale(model_trans, glm::vec3(0.2f));

    glUniformMatrix4fv(lamp_model_loc, 1, GL_FALSE, glm::value_ptr(model_trans));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();

    params::camera->update_time_deltas();
    process_input(window);
  }

  return R_SUCCESS;
}

/** ========================================================================= */
int r_draw_boxes(GLFWwindow *window)
/** ========================================================================= */
{
  glEnable(GL_DEPTH_TEST); 

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_drag_cb);
  glfwSetScrollCallback(window, mouse_scroll_cb);

  GLuint vao[] = { 0 };
  glGenVertexArrays(sizeof(vao) / sizeof(vao[0]), vao);
  glBindVertexArray(vao[0]);

  GLuint vbo[] = { 0 };
  glGenBuffers(sizeof(vbo) / sizeof(vbo[0]), vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_coords_w_textures), cube_coords_w_textures, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  GLint result = GL_TRUE;
  vector<GLchar> msg;

  GLuint texture1 = load_texture("nell_normal.jpg", result, msg, false);
  GLuint texture2 = load_texture("nell_haunted.jpg", result, msg, false);

  r_handle_result(result, msg);

  GLuint shader_programme = load_program("texture_box_vs.glsl", "texture_box_fs.glsl", result, msg);

  r_handle_result(result, msg);

  glUseProgram(shader_programme);
  params::visibility_loc = glGetUniformLocation(shader_programme, "vis");
  glUniform1f(params::visibility_loc, params::visibility_val);
  glUniform1i(glGetUniformLocation(shader_programme, "texture_sampler1"), 0);
  glUniform1i(glGetUniformLocation(shader_programme, "texture_sampler2"), 1);

  GLuint model_trans_loc = glGetUniformLocation(shader_programme, "model");
  GLuint view_trans_loc = glGetUniformLocation(shader_programme, "view");
  GLuint projection_trans_loc = glGetUniformLocation(shader_programme, "projection");

  glm::vec3 camera_pos(0.f, 0.f, 3.f);
  glm::vec3 camera_front(0.f, 0.f, -1.f);
  glm::vec3 world_up(0.f, 1.f, 0.f);
  params::camera = new Camera(window, camera_pos, camera_front, world_up);

  while (!glfwWindowShouldClose(window))
  {
    r_update_fps_counter(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat p = glfwGetTime();

    glm::mat4 view_trans, projection_trans;

    view_trans = params::camera->get_view();
    projection_trans = params::camera->get_projection();

    glUniformMatrix4fv(view_trans_loc, 1, GL_FALSE, glm::value_ptr(view_trans));
    glUniformMatrix4fv(projection_trans_loc, 1, GL_FALSE, glm::value_ptr(projection_trans));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glUseProgram(shader_programme);
    glBindVertexArray(vao[0]);

    for (int i = 0; i < 10; ++i)
    {
      glm::mat4 model_trans;

      model_trans = glm::translate(model_trans, cube_positions[i]);
      float angle = 20.0f * (i + 1) * p;
      model_trans = glm::rotate(model_trans, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      glUniformMatrix4fv(model_trans_loc, 1, GL_FALSE, glm::value_ptr(model_trans));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    params::camera->update_time_deltas();
    process_input(window);
    update_visibility(window);
  }

  return R_SUCCESS;
}

/** ========================================================================= */
void process_input(GLFWwindow *window)
/** ========================================================================= */
{
  if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE) ||
    GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ENTER))
    glfwSetWindowShouldClose(window, GL_TRUE);

  
  
  params::camera->process_keyboard_input();
}

void update_visibility(GLFWwindow *window)
{
  if (int key_u = glfwGetKey(window, GLFW_KEY_UP),
      key_d = glfwGetKey(window, GLFW_KEY_DOWN);
      GLFW_PRESS == key_u || GLFW_PRESS == key_d)
  {
    int dir = (key_u == GLFW_PRESS) ? 1 : -1;
    params::visibility_val += (params::visibility_speed * dir);
    params::visibility_val = clamp(params::visibility_val, 0.f, 1.f);
    glUniform1f(params::visibility_loc, params::visibility_val);
  }
}

void mouse_drag_cb(GLFWwindow * /** Ignore */, double xpos, double ypos)
{
  params::camera->set_cursor(xpos, ypos);
}

void mouse_scroll_cb(GLFWwindow * /** Ignore */, double /** Ignore */, double y_off)
{
  params::camera->zoom(y_off);
}


/** ========================================================================= */
int r_draw_texture_box(GLFWwindow *window)
/** ========================================================================= */
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

  int w, h;

  glfwGetWindowSize(window, &w, &h);

  glm::mat4 model_trans, view_trans, projection_trans;

  model_trans = glm::rotate(model_trans, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
  view_trans = glm::translate(view_trans, glm::vec3(0.f, 0.f, -3.f));
  projection_trans = glm::perspective(glm::radians(45.f), (float)w / (float)h, .1f, 100.f);

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

  GLuint model_trans_loc = glGetUniformLocation(shader_programme, "model");
  GLuint view_trans_loc = glGetUniformLocation(shader_programme, "view");
  GLuint projection_trans_loc = glGetUniformLocation(shader_programme, "projection");

  glUniformMatrix4fv(model_trans_loc, 1, GL_FALSE, glm::value_ptr(model_trans));
  glUniformMatrix4fv(view_trans_loc, 1, GL_FALSE, glm::value_ptr(view_trans));
  glUniformMatrix4fv(projection_trans_loc, 1, GL_FALSE, glm::value_ptr(projection_trans));

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
      int dir = (key_u == GLFW_PRESS) ? 1 : -1;
      visibility_val += (speed * dir);
      visibility_val = clamp(visibility_val, 0.f, 1.f);
      glUniform1f(visibility_loc, visibility_val);
    }


  }

  return R_SUCCESS;
}


/** ========================================================================= */
int r_draw_triangle(GLFWwindow *window)
/** ========================================================================= */
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