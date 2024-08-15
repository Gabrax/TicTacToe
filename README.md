# TicTacToe

 <p align="center">
    <img src="https://github.com/user-attachments/assets/a8ab5974-5cb1-43a9-b66b-ec65409d00a9" alt="TicTacToeLogo" width="200"/>
</p>

> [!WARNING]
> Game is not polished. Keep your expectations low.

The objective of this project is to gain experience in C++ and OpenGL development.

## 💬 To run this project, Conan 2.0 and Cmake needs to be installed

- First, choose which libraries build type u want:
```bash
conan install . -sbuild_type=Release -of="conan/rel" -s compiler.cppstd=20 --build=missing
```
```bash
conan install . -sbuild_type=Debug -of="conan/deb" -s compiler.cppstd=20 --build=missing
```
- Then, show the presets to choose from 
```bash
cmake --list-presets 
```
- Choose the build type (choose build type of libraries you've chosen in previous step)
```bash
cmake . --preset "<name-of-preset>"
```
- Lastly, build the project
```bash
cmake --build --preset "<name-of-preset>"
```

## 👨‍💻 Tech stack
- C++
- GLFW
- GLAD
- stb_image
- GLSL
- CMake
- Conan




