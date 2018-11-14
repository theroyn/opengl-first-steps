#ifndef R_CAMERA_H
#define R_CAMERA_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define CAMERA_EPSILON 0.0000001f
#define DEFAULT_ZOOM 45.

class Camera
{
public:
  Camera(const glm::vec3 &pos,
         const glm::vec3 &front,
         const glm::vec3 &world_up);

public:
  glm::mat4 get_view() const;
  void increase_pos(const glm::vec3 &pos);
  void update_view();
  void set_cursor(double x, double y);
  void process_keyboard_input(GLFWwindow *window);
  void reset_zoom();
  float get_fov() const;
  void zoom(double y_off);

private:
  void increase_pitch(double pitch);
  void increase_yaw(double yaw);
  void offset_angles(double xoff, double yoff);
  void step_forward(int dir);
  void step_right(int dir);
  glm::mat4 get_lookat() const;

private:
  double pitch_, yaw_;
  double fov_;
  float spin_speed_, translation_speed_, zoom_speed_;
  bool is_cursor_init_;

private:
  glm::vec3 world_up_;
  glm::vec3 pos_;
  glm::vec3 front_;
  glm::vec3 orig_front_;
  glm::vec3 up_;
  glm::vec2 cursor_;

};

#endif