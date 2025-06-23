# TheGameLabProject

🚧 ## Experimental Graphics Engine in C++
This is a personal learning project focused on building a minimal graphics engine from scratch in C++. The goal is to gain experience with low-level rendering, modern C++ architecture, and modular engine design.

The project includes:

✅ 3D model loading using Assimp

✅ Rendering pipeline built with OpenGL

✅ Shader management and basic transformation logic

✅ Scene setup and testing of rendering components

✅ Custom build system using Premake and Git submodules

## Objectives
Deepen understanding of 3D rendering pipelines

Explore modular architecture patterns in C++

Learn to manage complex build setups and third-party libraries

Gain practical experience with engine-level concepts

🧪 ## Project Status
This is a work in progress and not intended for production.
Some components are experimental or incomplete, and the architecture is subject to change as the project evolves.

🛠️ ## Tech Stack
Language: C++17

Rendering: OpenGL

Build System: Premake5

Dependencies: stb, Assimp (via Git submodules)

💡 ## Notes
This project is primarily educational. While it is not a commercial engine, it demonstrates my ability to:

Organize and scale a C++ codebase

Integrate and manage external dependencies

Understand graphics programming fundamentals

Work independently on complex, multi-module systems

## Download and compile

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
