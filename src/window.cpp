#include <sys/time.h>

#include "asseration.hpp"
#include "window.hpp"

void window_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  auto w = static_cast<Window *>(glfwGetWindowUserPointer(window));

  ASSERT(w);

  w->on_window_framebuffer_size(width, height);
}

void mouse_movement_callback(GLFWwindow *window, double xpos, double ypos)
{
  auto w = static_cast<Window *>(glfwGetWindowUserPointer(window));

  ASSERT(w);

  w->on_mouse_movement(xpos, ypos);
}

void Window::on_window_framebuffer_size(int width, int height)
{
  window_height = height;
  window_width  = width;
}

void Window::on_mouse_movement(double, double) {}

void Window::set_should_close(bool value)
{
  glfwSetWindowShouldClose(window, value);
}

Window::Window(const std::string &title,
               const unsigned     width,
               const unsigned     height)
    : title(title),
      window_width(width),
      window_height(height)
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
  start_time = get_current_time_millis();
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
  if (fullscreen)
    monitor = glfwGetPrimaryMonitor();

  window = glfwCreateWindow(window_width,
                            window_height,
                            title.c_str(),
                            monitor,
                            NULL);

  if (!window)
  {
    std::runtime_error("Could not create window");
  }

  glfwSetWindowUserPointer(window, this);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, window_framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_movement_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

int Window::get_key(int key) { return glfwGetKey(window, key); }

int Window::get_mouse_button(int button)
{
  return glfwGetMouseButton(window, button);
}

void Window::show()
{
  while (!glfwWindowShouldClose(window))
  {
    calcDeltaTime();
    ++frames_count;

    draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void Window::draw() {}

void Window::calcDeltaTime()
{
  float currentFrame = glfwGetTime();
  delta_time         = currentFrame - last_frame;
  last_frame         = currentFrame;
}

double Window::get_current_time_millis()
{
  timeval t;
  gettimeofday(&t, NULL);

  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return static_cast<double>(ret);
}

double Window::get_running_time()
{
  const auto runningTime = (get_current_time_millis() - start_time) / 1000.0;
  return runningTime;
}
