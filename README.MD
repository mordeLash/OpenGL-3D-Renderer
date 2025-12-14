# 3D Model Renderer in OpenGL

A computer graphics project by Mordechai Lashansky for my [Your Course Name, e.g., "Computer Graphics"] course at [Bar Ilan University]. This application is a 3D model viewer built using C++ and OpenGL, demonstrating a wide range of real-time rendering techniques.

## 🚀 Key Features

This renderer implements several fundamental and advanced graphics concepts from the ground up:

* **Phong Lighting Model:** Full implementation of the Phong reflection model, with real-time controls for:
    * **Ambient:** Global, non-directional light.
    * **Diffuse:** Light interacting with matte surfaces.
    * **Specular:** Shiny, mirror-like highlights.
* **Shading Techniques:**
    * **Gouraud Shading:** Per-vertex lighting calculation.
    * **Phong Shading:** Per-pixel lighting for smoother, more realistic results.
* **Advanced Mapping:**
    * **Texture Mapping:** Applies 2D texture files to 3D models.
    * **Normal Mapping:** Uses normal maps to simulate complex surface detail (like bumps and scales) without increasing polygon count.
* **Lighting & Scene:**
    * Support for multiple, colored light sources.
    * Toggleable point lights.
    * Back-face culling (one-sided vs. two-sided rendering).
* **Geometry:**
    * Vertex deformation.
    * Bézier curve rendering.

## 🛠️ Built With

* C++
* OpenGL
* freeGlut
* glew
* AntTweakBar (for the real-time GUI controls)

## ⚙️ How to Build and Run

*(Add your build instructions here. For example:)*
> This project was built using Visual Studio 2022. To run:
> 1.  Clone the repository.
> 2.  Ensure you have the dependencies (freeGlut, glew, AntTweakBar) linked.
> 3.  Open the `.sln` file and build the project in Release mode.
> 4.  Run the resulting `.exe`.

## 📦 Models Used

The 3D models (.obj files) and textures used in this project can be found at this Google Drive link: [https://drive.google.com/drive/folders/1SIrIEXSaEaZImYS71hjM-wQrTPKj1Y1F](https://drive.google.com/drive/folders/1SIrIEXSaEaZImYS71hjM-wQrTPKj1Y1F?usp=drive_link)