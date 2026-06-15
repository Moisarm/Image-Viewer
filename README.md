# 🖼️ Custom C Image Viewer (Runara & OpenGL)

A high-performance, low-overhead native image viewer built from scratch in modern **C**. The project utilizes **OpenGL 3.3 (Core Profile)** hardware acceleration via the **Runara** graphics library, directly handling window life-cycles, context management, and input polling through **GLFW** and **GLAD**.

This project was developed with a strict focus on modular systems architecture, GPU-side matrix manipulations, and memory safety in low-level programming.

---

## ✨ Key Features

- **⚡ GPU-Accelerated Rendering:** Ultra-fast texture loading and seamless image scaling without bottlenecking the CPU.
- **📐 Real-Time Graphical Manipulation (cglm):**
  - **Rotation:** Instant 90° increments offloaded completely to the GPU vertex shaders.
  - **Image Mirroring (Flip):** Native matrix inversion of texture coordinates ($U, V$) for both horizontal and vertical axes.
  - **Dynamic Zoom:** Smooth mouse scroll-wheel scaling bound to the viewport layout.
- **🖥️ Robust Window State Management:** Full-screen toggling (`F11`) that caches desktop geometry metrics safely within state context structures, preventing context-switching collapse bugs (`0x0`).
- **📁 Directory-Wide File Navigation:** Integrated directory indexing (`navigation.h`) that allows cycling through all valid images inside a folder using the keyboard arrow keys.
- **📊 Live Metadata Status Bar:** Sub-pixel text rendering powered by system fonts (`FreeType` + `HarfBuzz`) to display real-time telemetry: file name, source resolution, zoom factor, and directory index (`[4 / 23]`).

---

## 🛠️ Tech Stack & Dependencies

To ensure total control over memory allocations and binary size, external high-level overhead was explicitly minimized:

- **Language:** C (C11/C99 standard compliance)
- **Graphics API:** OpenGL 3.3 (Core Profile) & GLAD (Loader)
- **Windowing & Input:** GLFW3
- **Graphics Math:** `cglm` / `cglm-struct` (Highly optimized matrix-vector operations)
- **2D Graphics Engine:** Runara Core Engine
- **Typography:** FreeType 2 & HarfBuzz (Glyph generation and advanced text shaping)

---

## 📂 Project Architecture

The codebase adheres to a strict clean, decoupled structure, separating structural initialization from math calculations and event propagation pipelines:

```text
├── build/                      # Compiled binaries and translation units
├── include/
│   ├── callbacks/              # GLFW event handler signatures
│   ├── structs/
│   │   └── global-state.h      # Unified global runtime context (State pattern)
│   └── calculate-ratio-letterbox.h
├── src/
│   ├── callbacks/              # Key, Scroll, Resize, and Drag-and-Drop routines
│   ├── glad.c                  # OpenGL extension loader implementation
│   ├── image-viewer.c          # Primary Game Loop / Render Loop
│   ├── navigation.c
│   ├── save-image.c
│   └── calculate-ratio-letterbox.c
└── Makefile                    # Native Linux automation compilation script

⌨️ Controls & HotkeysKey / EventActionF11Toggle Fullscreen / Windowed mode smoothly while maintaining aspect ratio.RRotate image 90° clockwise (GPU accelerated).HToggle Horizontal Flip (Mirror texture layout).VToggle Vertical Flip (Invert texture layout).Right / Left ArrowStep forward or backward through the active directory index.Mouse Scroll-WheelDynamic Viewport Zoom In / Zoom Out.Drag & DropDrop any compatible image file directly onto the window to open it.🚀 Building & RunningPrerequisites (Linux / Debian / Kali)Ensure your environment has the required development libraries and header files installed:Bashsudo apt update
sudo apt install build-essential libglfw3-dev libfreetype6-dev libharfbuzz-dev
Compilation InstructionsThe project comes with an optimized Makefile configured to output safe binaries containing debug symbols (-g):Purge residual binaries:Bashmake clean
Compile the source code:Bashmake
Launch the viewer:Bashmake run
# Or invoke the binary directly:
./build/image-viewer

🧠 Solved Technical ChallengesMitigating Graphical Context Collapse (0x0 Window Bug): Engineered defensive logic inside the input callbacks to trap duplicate windowing state ticks. The architecture safeguards native resolution baselines prior to launching hardware monitor streams, ensuring the display window never collapses to an invalid width or height.Memory Pointer Boundaries in File Stripping: Implemented a robust strrchr parser pattern to cleanly isolate file string names from absolute paths. This eliminates segmentation faults commonly caused by accidental NULL pointer arithmetic.Layout Consistency (Culling & Letterboxing): Synchronized projection matrix updates to isolate UI viewports from texture dimensions. This prevents scaled graphics frames from drawing over or eclipsing the live telemetry overlay text bar.
```
