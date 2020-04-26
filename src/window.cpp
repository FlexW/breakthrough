#include <sys/time.h>

#include "asseration.hpp"
#include "window.hpp"

void windowFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  auto w = static_cast<Window *>(glfwGetWindowUserPointer(window));

  ASSERT(w);

  w->onWindowFramebufferSize(width, height);
}

void mouseMovementCallback(GLFWwindow *window, double xpos, double ypos)
{
  auto w = static_cast<Window *>(glfwGetWindowUserPointer(window));

  ASSERT(w);

  w->onMouseMovement(xpos, ypos);
}

void Window::onWindowFramebufferSize(int width, int height)
{
  windowHeight = height;
  windowWidth  = width;
}

void Window::onMouseMovement(double, double) {}

void Window::setShouldClose(bool value)
{
  glfwSetWindowShouldClose(window, value);
}

Window::Window(const std::string &title,
               const unsigned     width,
               const unsigned     height)
    : title(title), windowWidth(width), windowHeight(height)
{
  init();
}

Window::~Window()
{
  renderer->teardown();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Window::init()
{
  startTime = getCurrentTimeMillis();
  initWindow();
  initRenderer();
}

void Window::initRenderer()
{
  renderer->init((GLADloadproc)glfwGetProcAddress);
}

void Window::initWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 16);

  GLFWmonitor *monitor = NULL;
  if (isFullscreen)
    monitor = glfwGetPrimaryMonitor();

  window =
      glfwCreateWindow(windowWidth, windowHeight, title.c_str(), monitor, NULL);

  if (!window)
  {
    std::runtime_error("Could not create window");
  }

  glfwSetWindowUserPointer(window, this);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, windowFramebufferSizeCallback);
  glfwSetCursorPosCallback(window, mouseMovementCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

int Window::getKey(int key) { return glfwGetKey(window, key); }

int Window::getMouseButton(int button)
{
  return glfwGetMouseButton(window, button);
}

void Window::show()
{
  while (!glfwWindowShouldClose(window))
  {
    calcDeltaTime();
    ++framesCount;

    draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void Window::draw() {}

void Window::calcDeltaTime()
{
  float currentFrame = glfwGetTime();
  deltaTime          = currentFrame - lastFrame;
  lastFrame          = currentFrame;
}

double Window::getCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);

  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return static_cast<double>(ret);
}

double Window::getRunningTime()
{
  const auto runningTime = (getCurrentTimeMillis() - startTime) / 1000.0;
  return runningTime;
}
