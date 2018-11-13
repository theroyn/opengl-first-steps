#include "pch.h"
#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera(const glm::vec3 &pos,
               const glm::vec3 &front,
               const glm::vec3 &world_up) :
  pos_(std::move(pos)),
  orig_front_(std::move(front)),
  world_up_(std::move(world_up)),
  pitch_(0.),
  yaw_(0.),
  cursor_(0., 0.),
  zoom_speed_(3.f),
  fov_(DEFAULT_ZOOM),
  translation_speed_(0.005f),
  spin_speed_(.1f),
  is_cursor_init_(false)
{
  up_ = world_up_;
  front_ = orig_front_;
}


void Camera::step_forward(int dir)
{
  glm::vec3 forward = glm::normalize(front_);

  glm::vec3 inc = forward * translation_speed_ * ( float )dir;
  inc.y = 0;

  pos_ += inc;
  update_view();
}

void Camera::step_right(int dir)
{
  glm::vec3 forward = glm::normalize(front_);
  glm::vec3 right = glm::normalize(glm::cross(forward, world_up_));

  /**
   * right.y is always 0 because world_up_.xz are always(0,0).
   * If world_up_ changes an update will be neccessary.
   */
  pos_ += right * translation_speed_ * ( float )dir;
  update_view();
}

void Camera::increase_pos(const glm::vec3 &pos)
{
  pos_ += translation_speed_ * pos;
}

void Camera::process_keyboard_input(GLFWwindow *window)
{
  if (int key_u = glfwGetKey(window, GLFW_KEY_W),
      key_d = glfwGetKey(window, GLFW_KEY_S);
      GLFW_PRESS == key_u || GLFW_PRESS == key_d)
  {
    int dir = (key_u == GLFW_PRESS) ? 1 : -1;
    step_forward(dir);
  }

  if (int key_r = glfwGetKey(window, GLFW_KEY_D),
      key_l = glfwGetKey(window, GLFW_KEY_A);
      GLFW_PRESS == key_r || GLFW_PRESS == key_l)
  {
    int dir = (key_r == GLFW_PRESS) ? 1 : -1;
    step_right(dir);
  }
}

void Camera::zoom(double y_off)
{
  fov_ -= zoom_speed_ * y_off;

  if (double upper_lim = 180.; fov_ + CAMERA_EPSILON > upper_lim)
    fov_ = upper_lim - CAMERA_EPSILON;
  else if (fov_ - CAMERA_EPSILON < 0)
    fov_ = CAMERA_EPSILON;
}

void Camera::reset_zoom()
{
  fov_ = DEFAULT_ZOOM;
}

float Camera::get_fov()
{
  float fov = glm::radians(fov_);

  return fov;
}

void Camera::update_view()
{
  glm::mat4 m_pitch = glm::rotate(glm::mat4(), glm::radians(( float )pitch_), glm::vec3(1., 0., 0.));
  glm::mat4 m_yaw = glm::rotate(glm::mat4(), glm::radians(( float )yaw_)  , glm::vec3(0., 1., 0.));

  front_ = glm::normalize(m_yaw * m_pitch * glm::vec4(orig_front_, 1.f));
  glm::vec3 right = glm::normalize(glm::cross(front_, world_up_));
  up_ = glm::normalize(glm::cross(right, front_));
  /*cout << "front_:" << front_.x << ", " << front_.y << ", " << front_.z << endl;
  cout << "right:" << right.x << ", " << right.y << ", " << right.z << endl;
  cout << "up_:" << up_.x << ", " << up_.y << ", " << up_.z << endl;*/
}

glm::mat4 Camera::get_view() const
{
  glm::mat4 view = glm::lookAt(pos_, pos_ + front_, up_);

  return view;
}

void Camera::set_cursor(double x, double y)
{

  if (!is_cursor_init_)
  {
    cursor_.x = x;
    cursor_.y = y;
    is_cursor_init_ = true;
  }

  double xoff = x - cursor_.x;
  double yoff = y - cursor_.y;

  cursor_.x = x;
  cursor_.y = y;
  
  //increase_pitch(yoff);
  increase_yaw(xoff);
  offset_angles(xoff, yoff);
}

void Camera::offset_angles(double xoff, double yoff)
{
  pitch_ -= spin_speed_ * yoff;
  yaw_ -= spin_speed_ * xoff;

  if (pitch_ > 89.)
    pitch_ = 89.;
  else if (pitch_ < -89.)
    pitch_ = -89.;
}

void Camera::increase_pitch(double off)
{
  //cout << "pitch:" << pitch_ << ", off:" << spin_speed_ * off << endl;
  pitch_ -= spin_speed_ * off;

  if (pitch_ > 89.)
    pitch_ = 89.;
  else if (pitch_ < -89.)
    pitch_ = -89.;

  update_view();
}
void Camera::increase_yaw(double off)
{
  //cout << "yaw:" << yaw_ << ", radians(yaw):" << glm::radians(yaw_) << ", off:" << spin_speed_ * off << endl;
  //yaw_ = glm::mod(yaw_ + off, 360.);
  yaw_ -= spin_speed_ * off;

  update_view();
}