# CS0045 Group 2 OpenGL Project

## Technical Assessment 1

This repository contains the current CS0045 Technical Assessment 1 project for Modules 1–4. The previous Living Environment Explorer prototype has been removed so the repository now focuses on the required assessment activity.

The completed program displays three colorful shapes over a navy-to-plum gradient background. It demonstrates OpenGL primitives, per-vertex colors, vertex-array rendering, transformations, bitmap text, keyboard callbacks, timer-based animation, and double buffering.

## Current Program

The finished implementation is in [`Requirement1.cpp`](Requirement1.cpp).

- A movable purple gradient square drawn with `GL_QUADS`
- A green or cyan triangle rendered with a vertex array and `glDrawArrays`
- A continuously rotating multicolor diamond drawn with `GL_POLYGON`
- A four-corner gradient background that complements the shapes
- On-screen keyboard instructions using GLUT bitmap text
- Smooth animation using a timer callback and double buffering

The original partially completed starter is retained as [`CS0045_TA1.cpp`](CS0045_TA1.cpp) for comparison.

The standalone [`req5.cpp`](req5.cpp) snippet is also retained as a teammate contribution for the on-screen instruction function. Its completed implementation is integrated into `Requirement1.cpp`.

## Assessment Requirements

| Requirement | Implementation |
| --- | --- |
| At least two additional visible objects | Purple square and multicolor diamond |
| At least two OpenGL primitive types | `GL_QUADS`, `GL_TRIANGLES`, and `GL_POLYGON` |
| At least three colors | Purple, green/cyan, yellow, red, and blue |
| One object using vertex arrays | Center triangle |
| Keyboard interaction | Movement, color change, pause, reset, and exit controls |
| On-screen instructions | Two lines of GLUT bitmap text |

## Controls

| Key | Action |
| --- | --- |
| `A` | Move the purple square left |
| `D` | Move the purple square right |
| `C` | Switch the triangle between green and cyan |
| `Space` | Pause or resume the diamond rotation |
| `R` | Reset the square, triangle color, and diamond rotation |
| `Esc` | Close the program |

## Requirements

- Windows 10 or Windows 11
- Visual Studio Code with the Microsoft C/C++ extension
- MSYS2 installed at `C:\msys64`
- UCRT64 GCC, GDB, and FreeGLUT

Install the required packages from an MSYS2 UCRT64 terminal:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb mingw-w64-ucrt-x86_64-freeglut
```

## Build and Run in VS Code

1. Open this repository folder in Visual Studio Code.
2. Open `Requirement1.cpp`.
3. Press `Ctrl+Shift+B` to build the active C++ file.
4. Press `F5` to build and run it with the debugger.

The included `.vscode` configuration automatically builds the active `.cpp` file and launches the matching `.exe`.

## Manual Build

From PowerShell in the repository folder:

```powershell
& 'C:\msys64\ucrt64\bin\g++.exe' -g -std=c++17 '.\Requirement1.cpp' -o '.\Requirement1.exe' -lfreeglut -lopengl32 -lglu32
```

Then run:

```powershell
.\Requirement1.exe
```

## Project Structure

```text
CS0045-Group-2-OpenGL/
├── .vscode/
│   ├── c_cpp_properties.json
│   ├── launch.json
│   └── tasks.json
├── .gitignore
├── CS0045_TA1.cpp
├── Requirement1.cpp
├── req5.cpp
└── README.md
```

Compiled executables and debugger files are ignored by Git.

## Troubleshooting

### The `.exe` does not exist

Open `Requirement1.cpp` before pressing `F5`. The build task uses the currently active C++ file.

### FreeGLUT or compiler errors

Confirm these files exist:

```text
C:\msys64\ucrt64\bin\g++.exe
C:\msys64\ucrt64\bin\gdb.exe
C:\msys64\ucrt64\bin\freeglut.dll
```

If `freeglut.dll` is not found at runtime, add `C:\msys64\ucrt64\bin` to `PATH`.

## Team

Developed by CS0045 Group 2 for Technical Assessment 1.
