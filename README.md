## Description

C implementation of **[Langton's ant](https://en.wikipedia.org/wiki/Langton%27s_ant)** rendered in 3D with DDA ray cast using OpenGL

 <div align="center">
   <video src="https://github.com/user-attachments/assets/03aa873d-d03d-45cd-8f49-07fda4857315" width="100%"></video>
 </div>

---

### 🚀 Prerequisites
* CMake 3.21
* C99 compiler (GCC / Clang)
* OpenGL drivers (4.6, Core)

---

### 🛠️ Dependencies & Paths

#### System libs (configured via CMake)

* **GLFW3** -- Window creation and input handling
* **Freetype** -- Font rendering engine

#### Manual libs (configured via `.env.cmake`)

* 📦 **[GLAD](https://github.com/dav1dde/glad)** — OpenGL Loader Generator
  * Path: `$ENV{HOME}/Libs/glad`
* 📦 **[cglm v0.9.6](https://github.com/recp/cglm)** — Optimized 3D Math Library for C
  * Path: `$ENV{HOME}/Libs/cglm-0.9.6`
* 📦 **[stb_ds.h & stb_image.h](https://github.com/nothings/stb)** — Simple hashmap and image loading
  * Path: `$ENV{HOME}/Libs/stb`

---

### 🎨 Assets & Credits

These runtime assets are located in the `res/` directory.

#### 🔤 Fonts
* **[Minecraft Font](https://github.com/IdreesInc/Minecraft-Font/blob/main/Minecraft.otf)** — Used for HUD text elements. (License: OFL-1.1)

#### 🖼️ Textures
* **[Skyboxes](https://screamingbrainstudios.itch.io/cloudy-skyboxes-pack)** -- (License: Public Domain (CC0))

