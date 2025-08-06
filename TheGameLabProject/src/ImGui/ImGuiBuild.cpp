#include "pch.h"

// Compile the external dependencies that the ImGui library needs to be compiled for use
#include <misc/cpp/imgui_stdlib.cpp>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>