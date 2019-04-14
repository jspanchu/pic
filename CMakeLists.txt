cmake_minimum_required(VERSION 3.0)
 
PROJECT(tsi)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

target_include_directories(glad.c )
find_package(OpenGL)
find_package(X11)
 
add_executable(tsi  )

target_link_libraries(tsi ${GLFW_LIBRARIES} ${X11_LIBRARIES} ${OPENGL_LIBRARIES})
