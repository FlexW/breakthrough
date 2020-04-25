#pragma once

#include <string>

#include "renderer.hpp"

void windowFramebufferSizeCallback(GLFWwindow *window, int width, int height);

void mouseMovementCallback(GLFWwindow *window, double xpos, double ypos);

class Window
{
public:
  Window(const std::string &title,
         const unsigned     width  = 1280,
         const unsigned     height = 720);

  virtual ~Window();

  int getKey(int key);

  int getMouseButton(int button);

  void show();

  void setShouldClose(bool value);

protected:
  const std::string title;
  int               windowWidth  = 1280;
  int               windowHeight = 720;
  double            deltaTime    = 0.;
  Renderer          renderer;

  virtual void onWindowFramebufferSize(int width, int height);

  virtual void onMouseMovement(double xpos, double ypos);

  virtual void draw();

  double getRunningTime();

  double getCurrentTimeMillis();

  double             lastFrame   = 0.;
  double             startTime   = 0;
  unsigned long long framesCount = 0;

private:
  GLFWwindow *window       = nullptr;
  bool        isFullscreen = false;

  friend void
  windowFramebufferSizeCallback(GLFWwindow *window, int width, int height);

  friend void
  mouseMovementCallback(GLFWwindow *window, double xpos, double ypos);

  void init();

  void initWindow();

  void initRenderer();

  void calcDeltaTime();
};
