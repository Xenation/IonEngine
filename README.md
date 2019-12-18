# IonEngine

![Sample_01](https://raw.githubusercontent.com/wiki/Xenation/IonEngine/img/sample_01.png)

IonEngine is currently under developpement. It's main purpose is to experiment with several techniques.

### Features
- Scene Hierarchy
- Custom math library (uses SIMD instructions and Rotors for rotations)
- Deferred Renderer
- Clustered Shading (compute shader light assignment)
- Basic BRDF with metallic workflow
- Shader pipeline (detects layouts and possible permutations, uses UBOs to define Materials)
- Basic Shadows with Atlas (Directional and Spotlights supported)
- GUI using DearIMGUI
- Basic Bullet3 implementation

### Upcoming
- DX12 / Vulkan
- Better Shadows (Point light support, better smooth)
- Mesh Import pipeline
- Textured Skybox
- Handles to edit scene
- Scene file format