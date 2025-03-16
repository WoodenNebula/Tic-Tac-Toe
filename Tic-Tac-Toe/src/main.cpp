/*#include "glad/glad.h"*/
#include <iostream>
#include <GLFW/glfw3.h>

int main(int argc, char *argv[]) {
  if (!glfwInit())
  {
    std::cout << "[ERROR-GLAD] Initialization failed" << std::endl;
  }


  /*int x =   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);*/

  // creating a window

  int width = 800, height = 600;
  int x_pos = 0, y_pos = 0;
  const char *windowTitle = "Tic-Tac-Toe";

  // displaying single color
  glfwTerminate();
  return 0;
}
