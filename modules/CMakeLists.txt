add_subdirectory(glfw)
add_subdirectory(imgui-cmake)

if(NOT TARGET(eigen))
  set(EIGEN_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/eigen3)
  add_library(eigen INTERFACE)
  target_include_directories(eigen INTERFACE ${EIGEN_ROOT} ${EIGEN_ROOT}/unsupported)
endif()
