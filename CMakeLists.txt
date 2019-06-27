cmake_minimum_required (VERSION 3.0)

project(cloudviewer)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(modules)
add_subdirectory(src)

include(cmake/CMakeRC.cmake)
cmrc_add_resource_library(nimbus-shaders
    NAMESPACE shaders
    WHENCE shaders
    shaders/billboard.fs
    shaders/billboard.vs
    shaders/debug/mvp_color.fs
    shaders/debug/mvp_color.vs
    shaders/fragment.fs
    shaders/point_xyz_cloud.fs
    shaders/point_xyz_cloud.gs
    shaders/point_xyz_cloud.vs
    shaders/point_xyzrgb_cloud.fs
    shaders/point_xyzrgb_cloud.gs
    shaders/point_xyzrgb_cloud.vs
    shaders/simplefrag.fs
    shaders/vertex.vs
    )
