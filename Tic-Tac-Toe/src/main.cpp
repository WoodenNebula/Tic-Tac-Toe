#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>

static void key_callback(GLFWwindow *windowHandle, int key, int scancode,
                         int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(windowHandle, GLFW_TRUE);
  }
}

int main(int argc, char *argv[]) {
  // glfw should always be initialized first!!!
  if (!glfwInit()) {
    const char *error_desc = "";
    int error_code = glfwGetError(&error_desc);
    std::cerr << "[ERROR-GLFW] Initialization failed" << error_desc << ", "
              << error_code << std::endl;
    abort();
  }

  // set opengl version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // creating a window
  int width = 800, height = 600;
  int x_pos = 0, y_pos = 0;
  const char *windowTitle = "Tic-Tac-Toe";

  GLFWwindow *windowHandle =
      glfwCreateWindow(width, height, windowTitle, NULL, NULL);

  if (!windowHandle) {
    const char *error_desc = "";
    int error_code = glfwGetError(&error_desc);
    std::cerr << "[ERROR-GLFW] Window Initialization failed" << error_desc
              << ", " << error_code << std::endl;
    abort();
    abort();
  }

  glfwSetKeyCallback(windowHandle, key_callback);

  // this creates a valid window context for opengl to render to
  glfwMakeContextCurrent(windowHandle);

  // glad should only be initialized after a valid context has been created
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "[ERROR-GLAD] Initialization failed" << std::endl;
    abort();
  }

  while (!glfwWindowShouldClose(windowHandle)) {
    glClearColor(.69, .69, .69, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(windowHandle);
    glfwPollEvents();
  }

  // cleanup
  glfwDestroyWindow(windowHandle);
  glfwTerminate();
  return 0;
}
