#include "pch.h"
#include "RUTIL.h"


#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

string readFile(const char *file_path)
{
  string content;
  ifstream file_stream(file_path, ios::in);

  if (!file_stream.is_open())
  {
    cerr << "Could not read file " << file_path << ". File does not exist." << endl;
    return "";
  }

  string line = "";
  while (!file_stream.eof())
  {
    getline(file_stream, line);
    content.append(line + "\n");
  }

  file_stream.close();
  return content;
}

GLuint load_texture(const char *path, GLint &result, vector<GLchar> &msg, bool has_alpha)
{
  GLuint texture = 0;
  int w = 0, h = 0, nc = 0;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(path, &w, &h, &nc, 0);

  if (data == NULL)
  {
    result = R_FAILURE;

    string err_msg = "Texture loading failed.";
    msg.resize(err_msg.length());
    strcpy_s(&msg[0], err_msg.length(), err_msg.c_str());
  }
  else
  {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, has_alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }

  return texture;
}

GLuint load_shader(const char *path, GLenum type, GLint &result, std::vector<GLchar> &msg)
{
  GLuint shader = glCreateShader(type);
  string shaderStr = readFile(path);
  const char *shaderSrc = shaderStr.c_str();

  int logLength;

  // Compile vertex shader
  dbg_print("Compiling shader.");
  glShaderSource(shader, 1, &shaderSrc, NULL);
  glCompileShader(shader);

  // Check shader
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
  {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    msg.resize(logLength);
    glGetShaderInfoLog(shader, logLength, NULL, &msg[0]);
  }
  else
  {
    dbg_print("Successful compilation.");
  }

  return shader;
}

GLuint load_program(GLuint vertShader, GLuint fragShader, GLint &result, std::vector<GLchar> &msg)
{
  GLuint program = glCreateProgram();

  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &result);

  if (result == GL_FALSE)
  {
    int log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
    msg.resize(log_length);
    glGetProgramInfoLog(program, log_length, NULL, &msg[0]);
  }

  return program;
}


GLuint load_program(const char *vertex_path, const char      *fragment_path,
                    GLint      &result,      vector<GLchar>  &msg,
                    GLuint     *vs,          GLuint          *fs)
{
  GLuint program = 0;
  GLuint vertShader = load_shader(vertex_path, GL_VERTEX_SHADER, result, msg);
      

  if (result == GL_TRUE)
  {
    GLuint fragShader = load_shader(fragment_path, GL_FRAGMENT_SHADER, result, msg);
          
    if (result == GL_TRUE)
    {
      program = load_program(vertShader, fragShader, result, msg);

      if (vs == NULL)
        glDeleteShader(vertShader);
      else
        *vs = vertShader;

      if (fs == NULL)
        glDeleteShader(fragShader);
      else
        *fs = fragShader;
    }
  }

  return program;
}

void dbg_print(const string &msg)
{
#ifdef _DEBUG
  cout << "Debug Message:" << endl << msg << endl;
#endif // DEBUG
}

int r_exit(int code, const string &msg)
{
  if (!msg.empty())
    dbg_print(msg);

  glfwTerminate();

  cout << endl << "Press Enter to exit with code " << code << "..." << endl;

  /**cin.clear();
  cin.get();*/


  return code;
}

static void _update_fps_counter(GLFWwindow* window)
{
  static double previous_seconds = glfwGetTime();
  static int frame_count;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - previous_seconds;
  if (elapsed_seconds > 0.25)
  {
    previous_seconds = current_seconds;
    double fps = (double)frame_count / elapsed_seconds;
    char tmp[128];
    sprintf_s(tmp, "opengl @ fps: %.2f", fps);
    glfwSetWindowTitle(window, tmp);
    frame_count = 0;
  }
  frame_count++;
}

void r_update_fps_counter(GLFWwindow* window)
{
#ifdef _DEBUG
  _update_fps_counter(window);
#endif
}