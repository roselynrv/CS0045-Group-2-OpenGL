# CS0045 Group 2 OpenGL Project

## Living Environment Explorer

Living Environment Explorer is an interactive 2D OpenGL environmental simulation. The player controls a pixel-style robot tamaraw, explores three animated habitats, and completes activities that visibly improve each environment.

The project uses the fixed-function OpenGL and FreeGLUT concepts covered in CS0045 Modules 1–4, including primitives, colors, bitmap text, keyboard and mouse callbacks, timers, and vertex arrays.

## Main Features

- Three switchable environments: Living Forest, Coastal Wetland, and Eco Campus
- Animated robot tamaraw with walking, flying, jumping, sleeping, facial expressions, and outfits
- Proximity-based interaction prompts and outlines
- Environmental activities with visible before-and-after changes
- Animated water, clouds, birds, plants, and background scenery
- Information panels that automatically disappear after five seconds
- Counters for restored habitats, water, food, and discoveries
- Ready-to-use VS Code build and debug configuration

## Environments and Activities

### Living Forest

- **River:** Restores stones and improves the animated stream.
- **Tree Canopy:** Grows a denser group of trees.
- **Forest Floor:** Adds a fallen log, mushrooms, ferns, and ground details.
- Includes a dim forest atmosphere, layered trunks, dense foliage, and perched birds.

### Coastal Wetland

- **Wetland Water:** Restores aquatic plants and wetland life.
- **Mangroves:** Improves the mangrove habitat.
- **Migratory Birds:** Attracts more animated birds.
- Includes a nighttime sky, crescent moon, rocky mountains, refined clouds, animated water ripples, and moon reflections.

### Eco Campus

- **Solar Building:** Activates solar-powered building lights.
- **Community Garden:** Adds flowers and pollinators.
- **Recycling Hub:** Sorts waste and powers all campus lamps and building lights.
- Includes a sunset skyline, solar panels, raised garden beds, recycling bins, walkways, landscaping, and street lamps.

## Controls

| Key | Action |
| --- | --- |
| `W` | Move upward and enter the flying pose |
| `A` | Move left |
| `S` | Move downward and return to the ground |
| `D` | Move right |
| `Space` | Jump using the flying pose |
| `E` | Interact with a nearby habitat object |
| `C` / `V` | Previous or next environment |
| `I` / `K` | Previous or next facial expression |
| `J` / `L` | Previous or next outfit |
| `R` | Reset the tamaraw position |
| `Esc` | Exit the application |

The tamaraw falls asleep after approximately five seconds without keyboard or mouse input. Moving, clicking, or pressing a key wakes it immediately.

## Requirements

- Windows 10 or Windows 11
- Visual Studio Code
- Microsoft C/C++ extension for VS Code
- MSYS2 installed in `C:\msys64`
- UCRT64 GCC, GDB, and FreeGLUT packages

Install the required MSYS2 packages from an MSYS2 UCRT64 terminal:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb mingw-w64-ucrt-x86_64-freeglut
```

## Build and Run in VS Code

1. Open the `LivingEnvironmentExplorer` folder in Visual Studio Code. Do not open only the `.cpp` file.
2. Confirm that the Microsoft C/C++ extension is installed.
3. Press `Ctrl+Shift+B` to build the project.
4. Press `F5` and select **Run Living Environment Explorer** to build and launch the game with the debugger.

The supplied `.vscode` configuration expects these files:

```text
C:\msys64\ucrt64\bin\g++.exe
C:\msys64\ucrt64\bin\gdb.exe
C:\msys64\ucrt64\bin\freeglut.dll
```

## Manual Build

From PowerShell inside `LivingEnvironmentExplorer`:

```powershell
& 'C:\msys64\ucrt64\bin\g++.exe' -Wall -Wextra -g -std=c++17 '.\LivingEnvironmentExplorer.cpp' -o '.\LivingEnvironmentExplorer.exe' -lfreeglut -lopengl32 -lglu32
```

Run the compiled program:

```powershell
.\LivingEnvironmentExplorer.exe
```

If Windows reports that `freeglut.dll` is missing, add `C:\msys64\ucrt64\bin` to `PATH` or copy `freeglut.dll` beside `LivingEnvironmentExplorer.exe`.

## Project Structure

```text
CS0045-Group-2-OpenGL/
├── README.md
├── CS0045_TA1.cpp
├── req1.cpp
└── LivingEnvironmentExplorer/
    ├── LivingEnvironmentExplorer.cpp
    └── .vscode/
        ├── c_cpp_properties.json
        ├── launch.json
        └── tasks.json
```

`CS0045_TA1.cpp` and `req1.cpp` are retained from the earlier assessment work. The complete interactive simulation is located in `LivingEnvironmentExplorer`.

## Interaction Notes

- Habitat outlines are hidden until the tamaraw is close enough to interact.
- The `E - INTERACT` prompt identifies the available activity.
- Pressing `E` completes the activity and opens its information panel.
- The information panel remains visible for five seconds and then closes automatically.
- Completed activities remain restored when switching between scenes during the same run.

## Troubleshooting

### `launch: program ... does not exist`

Build first with `Ctrl+Shift+B`, then press `F5`. Also confirm that VS Code opened the `LivingEnvironmentExplorer` folder.

### Compiler or debugger not found

Verify that MSYS2 is installed at `C:\msys64` and that GCC and GDB exist inside `C:\msys64\ucrt64\bin`.

### OpenGL or FreeGLUT link errors

Install the UCRT64 FreeGLUT package and use the supplied build task. The link order already includes `freeglut`, `opengl32`, and `glu32`.

## Team

Developed by CS0045 Group 2 for Technical Assessment 1.
