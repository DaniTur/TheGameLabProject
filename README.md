# TheGameLabProject

## 🚀 Experimental Graphics Engine in C++
A learning-focused engine with basic 3D rendering using OpenGL, model loading, scene management, and real-time debugging tools.
This is a personal learning project focused on building a minimal graphics engine from scratch in C++. The goal is to gain experience with low-level rendering, modern C++ architecture, and modular engine design.

## 🔧 Features

- 3D rendering with OpenGL 3.3
- Custom build system using Premake5
- Model loading with Assimp
- Logging system using spdlog
- Real-time UI with ImGui
- Organized modular architecture in C++17/20

## 🚧 Project Status

Still under development. Some features are experimental or incomplete.
Primary goal: explore architecture and graphics techniques in a low-level engine.

## 🛠️ Tech Stack
Language: C++17/20

Rendering: OpenGL + GLFW + Glad

Build System: Premake5 + Git submodules

Dependencies: Assimp, spdlog, stb, ImGui

## 📸 Screenshots
![image](https://github.com/user-attachments/assets/922bd478-6d79-46ad-9658-63693634f1db)

## 📦 Download and compile

Download the project with the required submodeules:
``
git clone --recurse-submodules https://github.com/DaniTur/TheGameLabProject
``

If you cloned the repository without submodules, download submodules:
``
git submodule update --init --recursive
``

Execute the script to generate the project(uses premake5):
``
scripts/GenerateProject.bat
``

Open the Visual Studio solution file `TheGameLabProject.sln`

Build Solution with **Release x64** mode

Execute
