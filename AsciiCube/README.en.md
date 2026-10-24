<p align="right">
  <a href="README.md"><img src="https://img.shields.io/badge/%ED%95%9C%EA%B5%AD%EC%96%B4-30363D?style=for-the-badge" alt="한국어"></a>
  <a href="README.en.md"><img src="https://img.shields.io/badge/English-1F6FEB?style=for-the-badge" alt="English (selected)"></a>
</p>

# ASCII Cube
This ASCII cube demo demonstrates HiwoongEngine's 3D math and software rendering.
It transforms vertices on the CPU and fills triangles with character cells to display a rotating solid in the Windows console.
Characters change with the orientation of each face, while depth comparisons provide a sense of volume and occlusion.

<br>
</br>

## Demo

https://github.com/user-attachments/assets/639897e7-3dc8-43dd-8a19-020b15c6207a

This recording shows Model·View·Projection transformations, triangle filling, a depth buffer, and normal-based ASCII lighting.

## Scene and Cube

[CubeScene](Test/CubeScene.cpp) reads the screen dimensions from the engine settings and creates one cube.
[MeshFactory::CreateCube](../HiwoongEngine/src/Render/MeshFactory.cpp) generates the cube's geometry.

| Property | Current value |
|---|---|
| Cube side length | `2.0` |
| Local vertex range | `-1` to `1` on each axis |
| Geometry | 8 vertices, 12 edges, 12 triangles |
| Face structure | Each of the 6 square faces is represented by 2 triangles |
| Initial position | `(0, 0.5, 5)` |
| Initial rotation | `(0, 0, 0)` |
| Initial scale | `(1, 1, 1)` |
| Rotation speed | X axis `0.5`, Y axis `0.8`, Z axis `0.0` radians/second |

The current display fills triangle interiors. Edge data is included in the mesh but is not used for wireframe rendering.

## Object and Component Responsibilities

| Object or component | Responsibility |
|---|---|
| [CubeObject](Cube/CubeObject.cpp) | Stores the cube mesh, sets the initial Transform, and handles 3D drawing in `Draw()` |
| [CubeRotationComponent](Cube/CubeRotationComponent.cpp) | Updates the Transform's rotation each frame by multiplying the rotation speed by `deltaTime` |
| [TransformComponent](../HiwoongEngine/src/Component/TransformComponent.cpp) | Stores position, rotation, and scale, and calculates the Model matrix |
| [SoftwareRasterizer](../HiwoongEngine/src/Render/SoftwareRasterizer.cpp) | Converts screen coordinates, tests back faces, fills triangles, and calculates interpolation weights |
| [Renderer](../HiwoongEngine/src/Render/Renderer.cpp) | Collects submitted characters and depths to compose and output the final frame |

`GameObject` creates the default `TransformComponent`, and `CubeObject::Start()` adds `CubeRotationComponent`.
The rotation component only changes rotation values; it does not handle the mesh or console. `CubeObject` draws the next frame using the updated Model matrix.

The engine also has a shared 3D drawing class called [MeshRenderer](../HiwoongEngine/src/Render/MeshRenderer.h).
This class does not inherit from Component. It is a helper that draws a mesh using a Model matrix and `RenderView`.
The current Cube does not call `MeshRenderer`; it uses its own rendering path inside `CubeObject::Draw()`.

## From Vertices to Characters

The rendering flow is implemented in [CubeObject::Draw()](Cube/CubeObject.cpp).

1. **Model transformation**: Gets the cube's position, rotation, and scale matrix from `TransformComponent`.
2. **View·Projection transformation**: Multiplies by the fixed camera and perspective projection matrices to transform vertices into clip coordinates.
3. **Perspective divide**: Divides each coordinate by `w` to obtain normalized device coordinates (NDC).
4. **Screen coordinate conversion**: Maps NDC to console cell coordinates, then subtracts 15 from the Y coordinate to position the cube higher on the screen.
5. **Lighting and back-face testing**: Rotates each triangle's normal to calculate brightness and skips triangles that face away on the screen.
6. **Triangle filling**: Checks cells within the screen bounds to find positions inside the triangle.
7. **Depth interpolation and submission**: Interpolates NDC depth using each cell's barycentric weights and passes the character, color, and depth to `SubmitPoint3D()`.
8. **Frame output**: Renderer compares depths at the same cell, and the console output system alternates between two screen buffers.

The Model matrix is composed as `T × Rz × Ry × Rx × S`, and the full vertex transformation is `Projection × View × Model`.
When applied to column vectors, the rightmost matrix acts first, so transformations occur in the order of scaling, rotation, then translation.

Barycentric weights express a point inside a triangle as a weighted sum of its three vertices.
Cube applies these weights to the depths of the three vertices. Every cube cell has a `sortingOrder` of `0`; among cells with the same order, the smaller depth is retained.

## ASCII Lighting

The local normal is calculated by taking the cross product of two triangle edges, then its world direction is calculated using the rotation matrix.
The light direction is the normalized vector `(-1, -1, -1)`. The dot product of the normal and light direction is clamped to `0~1` to calculate diffuse lighting.

```text
Brightness = 0.2 + 0.8 × Diffuse lighting
Character ramp = " .:-=+*#%@"
```

The calculated brightness is clamped to `0~1` and then converted to an index into the character ramp.
The color is fixed to white, with character density representing brightness. The same character is used throughout each triangle.

## Camera and Projection Settings

Without a separate `Camera3D` object, Cube creates a fixed View matrix in `Draw()`.

| Property | Current value |
|---|---|
| Camera position | `(0, 0, 0)` |
| Look-at point | `(0, 0, 1)` |
| Up vector | `(0, -1, 0)` |
| Field of view | `π / 3` radians, approximately 60 degrees |
| Near and far projection values | `0.1`, `100.0` |
| Aspect ratio | Accounts for the actual character size ratio as well as the screen's cell width-to-height ratio |

If the character size cannot be obtained, the `characterAspectRatio` from [Setting.txt](../HiwoongEngine/Config/Setting.txt) is used.
The current settings file specifies a screen size of `240 × 67`, a target frame rate of `60`, and a fallback character aspect ratio of `0.5`.

Cube's own rendering path checks whether each vertex has `w > 0`, but does not clip triangles that cross the near plane.
The engine's shared [MeshRenderer implementation](../HiwoongEngine/src/Render/MeshRenderer.cpp) separately implements near-plane clipping and spotlight calculations.
