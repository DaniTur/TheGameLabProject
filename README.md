# TheGameLabProject

## 🗺️ 3D Map Editor in C++
An experimental map editor with simple 3D rendering using OpenGL, model loading, scene management, and real-time debugging tools.  
The purpose of the project is to create a functional editor to design and simulate scenarios, built on top of a custom lightweight engine developed in modern C++.

## 🔧 Features

- 3D rendering with OpenGL 3.3
- Custom build system using Premake5
- Model loading with Assimp
- Logging system using spdlog
- Real-time UI with ImGui
- Organized modular architecture in C++20

## 🚧 Project Status

Still under development. Some features are experimental or incomplete.  
The primary goal is to evolve into a useful **map editor for creating and simulating 3D scenarios**, while serving as a playground for learning rendering and engine architecture.

## 🛠️ Tech Stack
- **Language:** C++20  
- **Rendering:** OpenGL 3.3 + Glad + GLFW (Window)  
- **Build System:** Premake5 + Git submodules  
- **Dependencies:** Assimp, spdlog, stb, ImGui  

## 📸 Screenshots
![image](<img width="1280" height="751" alt="image" src="https://github.com/user-attachments/assets/61768e85-05f5-4ad9-873c-b07ab5227131" />)

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
