### 🚀 Prerequisites
* CMake 3.21
* C99 compiler (GCC / Clang)
* OpenGL drivers

---

### 🛠️ Dependencies & Paths

#### System libs (configured via CMake)

* **GLFW3** -- Window creation and input handling
* **Freetype** -- Font rendering engine

#### Manual libs (configured via `.env.cmake`)

* 📦 **[GLAD](https://dav1d.de)** — OpenGL Loader Generator
  * Path: `$ENV{HOME}/Libs/glad`
* 📦 **[cglm v0.9.6](https://github.com)** — Optimized 3D Math Library for C
  * Path: `$ENV{HOME}/Libs/cglm-0.9.6`
* 📦 **[stb_ds.h & stb_image.h](https://github.com)** — Simple hashmap and image loading
  * Path: `$ENV{HOME}/Libs/stb`
* 📦 **[fast_obj v1.3](https://github.com)** — Fast Wavefront OBJ loader
  * Path: `$ENV{HOME}/Libs/fast_obj-1.3`

---

### 🎨 Assets & Credits

These runtime assets are located in the `res/` directory.

#### 🔤 Fonts
* **[Minecraft Font](https://github.com/IdreesInc/Minecraft-Font/blob/main/Minecraft.otf)** — Used for HUD text elements. (License: OFL-1.1)

#### 🖼️ Textures
* **[Skyboxes](https://screamingbrainstudios.itch.io/cloudy-skyboxes-pack)** -- (License: Public Domain (CC0))

