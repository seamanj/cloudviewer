#include "window_helpers.hpp"


static void glfw_error_callback(int error, const char* description) {
  std::cerr << "Glfw error " << error << ": " << description << std::endl;
}


void initialize_glfw() {
  static bool initialized = false;
  if (initialized) {
    return;
  }

  glfwSetErrorCallback(glfw_error_callback);
  glfwInit();
  initialized = true;
}


GLFWwindow* initialize_window(size_t width, size_t height) {
  initialize_glfw();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  auto window = glfwCreateWindow(width, height, "ImGui GLFW+OpenGL3 example", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress));

  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  return window;
}


void update(Scene& scene) {
  glfwPollEvents();

  // Start the ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  scene.update();
}


void render(GLFWwindow *window) {
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void cleanup(GLFWwindow *window) {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}


void start_new_frame(GLFWwindow *window) {
  int display_w, display_h;
  glfwMakeContextCurrent(window);
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(0.0f, 0.05f, 0.15f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);
}


void finish_new_frame(GLFWwindow *window) {
  glfwMakeContextCurrent(window);
  glfwSwapBuffers(window);
}
