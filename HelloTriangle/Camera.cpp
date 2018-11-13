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
  translation_speed_(0.005f),
  spin_speed_(0.1f),
  is_cursor_init_(false)
{
  up_ = world_up_;
  front_ = orig_front_;
}


void Camera::step_forward(int dir)
{
  glm::vec3 forward = glm::normalize(front_);
  glm::vec3 right = glm::normalize(glm::cross(forward, world_up_));
  // glm::vec3 up = glm::normalize(glm::cross(right, forward));

  glm::vec3 inc = forward * translation_speed_ * ( float )dir;
  inc.y = 0;

  pos_ += inc;
  update_view();
}

void Camera::step_right(int dir)
{
  glm::vec3 forward = glm::normalize(front_);
  glm::vec3 right = glm::normalize(glm::cross(forward, world_up_));
  // glm::vec3 up = glm::normalize(glm::cross(right, forward));

  // right.y is always 0 because world_up_.xz are always(0,0).
  // If world_up_ changes an update will be neccessary.
  pos_ += right * translation_speed_ * ( float )dir;
  update_view();
}

void Camera::increase_pos(const glm::vec3 &pos)
{
  pos_ += translation_speed_ * pos;
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

  //// Calculate the new Front vector
  //glm::vec3 front;
  //front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  //front.y = sin(glm::radians(pitch_));
  //front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  //front_ = glm::normalize(front);
  //// Also re-calculate the Right and Up vector
  //glm::vec3 Right = glm::normalize(glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  //up_ = glm::normalize(glm::cross(Right, front_));
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