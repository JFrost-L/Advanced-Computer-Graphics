# Advanced Computer Graphics Project

This repository contains a collection of projects and code samples related to **Advanced Computer Graphics** techniques. The focus is on implementing and exploring cutting-edge graphics algorithms and methods used in modern 3D rendering engines, such as ray tracing, global illumination, physically-based rendering (PBR), and shader programming.

## Project Overview

Computer Graphics is a field that creates, manipulates, and renders visual content in two or three dimensions. This project dives into **advanced techniques** that are essential for creating realistic and high-performance graphics used in video games, animation, simulations, and virtual environments. The repository includes the implementation of various algorithms and techniques, as well as code to help visualize and understand the underlying principles.

### Key Topics Covered

1. **Ray Tracing**: A rendering technique that simulates the way light interacts with objects, allowing for accurate shadows, reflections, and refractions.
2. **Global Illumination**: Simulates indirect lighting, where light bounces off surfaces to illuminate other objects, creating a more realistic scene.
3. **Physically Based Rendering (PBR)**: A shading and rendering technique that models light interactions based on the physical properties of materials to produce highly realistic images.
4. **Shader Programming**: Writing custom GPU programs using **GLSL** (OpenGL Shading Language) to control the way vertices and fragments (pixels) are processed.
5. **Shadow Mapping**: A technique for adding shadows to 3D scenes by rendering the scene from the light source's perspective.
6. **Anti-Aliasing**: Techniques to reduce the visual defects (aliasing) in rendered images, including MSAA, FXAA, and SMAA.
7. **Procedural Texturing**: Generating textures algorithmically instead of using pre-existing bitmap images, allowing for endless variation and dynamic adjustments.

## Key Components of This Repository

1. **Ray Tracing Implementation**:
   - Implementation of a basic ray tracer that handles reflection, refraction, and shadows.
   - **Acceleration structures** like **Bounding Volume Hierarchies (BVH)** for improving performance in complex scenes.
   - Rendering complex shapes, reflections, and materials like glass or metal using physically accurate light transport models.

2. **Physically-Based Rendering (PBR)**:
   - Use of **BRDF (Bidirectional Reflectance Distribution Function)** to model how light interacts with different types of surfaces.
   - Accurate materials such as metals, plastics, and fabrics using PBR techniques.
   - Realistic lighting setups using **High Dynamic Range (HDR)** and image-based lighting.

3. **Global Illumination Techniques**:
   - **Path Tracing**: A Monte Carlo method for simulating global illumination by tracing multiple light paths.
   - **Radiosity**: A method for simulating diffuse light bouncing between surfaces in a scene.

4. **Shader Programming**:
   - Custom shaders written in **GLSL** for vertex, fragment, and geometry stages.
   - Procedural generation of textures and visual effects using shaders.
   - Implementation of advanced techniques like normal mapping, bump mapping, and ambient occlusion in real-time rendering.

5. **Anti-Aliasing Techniques**:
   - Implementation of **Multi-Sample Anti-Aliasing (MSAA)** to reduce jagged edges.
   - Use of **Fast Approximate Anti-Aliasing (FXAA)** for real-time performance in games.

6. **Rendering Optimization**:
   - Techniques such as **level of detail (LOD)**, **frustum culling**, and **deferred shading** to optimize rendering performance in real-time environments.

## Technologies Used

- **C++**: Main programming language for performance-critical 3D rendering tasks.
- **OpenGL**: Graphics API used for rendering 2D and 3D vector graphics.
- **GLSL**: Shader programming language used to control the graphics pipeline.
- **Assimp**: A library for importing and exporting 3D models from various formats.
- **Eigen**: A C++ template library for linear algebra, used for matrix and vector computations.
- **ImGui**: For creating in-app user interfaces for debugging and parameter tuning.
- **CUDA** (Optional): For parallel computation of ray tracing using GPU acceleration.
- **STB Image**: For loading and manipulating image files for textures.

## How to Run

To run this project locally, follow these steps:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/advanced-computer-graphics.git
   cd advanced-computer-graphics
