#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Engine::GLFWEventCallbacks
{
void key_callback(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);

void mouse_button_callback(GLFWwindow* windowHandle, int button, int action, int mods);

void mouse_scroll_callback(GLFWwindow* windowHandle, double xoffset, double yoffset);

void mouse_move_callback(GLFWwindow* windowHandle, double xoffset, double yoffset);

void window_pos_callback(GLFWwindow* windowHandle, int xpos, int ypos);

void window_size_callback(GLFWwindow* windowHandle, int width, int height);

void window_close_callback(GLFWwindow* windowHandle);

void window_focus_callback(GLFWwindow* windowHandle, int focused);
};