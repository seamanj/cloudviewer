add_library(nimbus
    nimbus/Scene.hpp
    nimbus/entities/Entity.cpp
    nimbus/entities/Camera.hpp
    nimbus/entities/Entity.hpp
    nimbus/entities/FixedShaderEntity.hpp
    nimbus/entities/PointXYZCloudEntity.hpp
    nimbus/entities/PointXYZCloudEntity.cpp
    nimbus/entities/AxesEntity.hpp
    nimbus/entities/AxesEntity.cpp
    nimbus/entities/Camera.cpp
    nimbus/Scene.cpp
    nimbus/shaders/CameraShaderProgram.hpp
    nimbus/shaders/MvpShader.hpp
    nimbus/shaders/Shader.cpp
    nimbus/shaders/PointXYZCloudShader.cpp
    nimbus/shaders/PointXYZCloudShader.hpp
    nimbus/shaders/MvpShader.cpp
    nimbus/shaders/Shader.hpp
    nimbus/shaders/Uniform.hpp
    nimbus/utils/Color.cpp
    nimbus/utils/Color.hpp
    nimbus/utils/math.hpp
    nimbus/glitter.hpp
    nimbus/SimplePclConverter.hpp
    nimbus/SimplePclConverter.cpp
    )

target_include_directories(nimbus PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(nimbus PUBLIC
    glfw
    eigen
    nimbus-shaders
    )

add_executable(${PROJECT_NAME}
    main.cpp

    imgui/imgui_impl_opengl3.h
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_glfw.cpp

    glad/glad.h
    glad/glad.c

    KHR/khrplatform.h

    Image.hpp
    DepthFrame.hpp
    DepthFrame.cpp
    TimeSeriesFrame.hpp
    Log.hpp
    PointCloud.cpp PointCloud.hpp window_helpers.cpp window_helpers.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(${PROJECT_NAME} PUBLIC
    imgui
    nimbus
    )
