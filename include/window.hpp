#pragma once

#include <string>

#include "renderer.hpp"

void window_framebuffer_size_callback(GLFWwindow *window,
                                      int         width,
                                      int         height);

void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos);

class Window
{
public:
  Window(const std::string &title,
         const unsigned     width  = 1280,
         const unsigned     height = 720);

  virtual ~Window();

  int get_key(int key);

  int get_mouse_button(int button);

  void show();

  void set_should_close(bool value);

protected:
  const std::string         title;
  int                       window_width  = 1280;
  int                       window_height = 720;
  double                    delta_time    = 0.;
  std::shared_ptr<Renderer> renderer;

  virtual void on_window_framebuffer_size(int width, int height);

  virtual void on_mouse_movement(double xpos, double ypos);

  virtual void draw();

  double get_running_time();

  double get_current_time_millis();

  double get_time();

  double             last_frame   = 0.;
  double             start_time   = 0;
  unsigned long long frames_count = 0;

private:
  GLFWwindow *window     = nullptr;
  bool        fullscreen = false;

  friend void
  window_framebuffer_size_callback(GLFWwindow *window, int width, int height);

  friend void
  mouse_movement_callback(GLFWwindow *window, double xpos, double ypos);

  void init();

  void initWindow();

  void initRenderer();

  void calcDeltaTime();
};
