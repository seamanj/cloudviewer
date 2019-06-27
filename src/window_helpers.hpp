#pragma once

#include <iostream>

/* Order of includes is IMPORTANT - don't change. imgui, then glad, then glfw. */
#include <imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Eigen/OpenGLSupport>

#include <nimbus/Scene.hpp>


GLFWwindow* initialize_window(size_t width, size_t height);

void update(Scene& scene);

void render(GLFWwindow* window);
void cleanup(GLFWwindow* window);

void start_new_frame(GLFWwindow* window);
void finish_new_frame(GLFWwindow* window);
