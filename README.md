# Solar System Simulation

A real-time N-body gravitational simulation and GPU ray-traced renderer written in C using OpenGL and GLSL.

The project simulates gravitational interactions between celestial bodies, renders them as ray-traced spheres with lighting and shadows, and includes an optional deformable grid that visualises how mass might distort spacetime.

> This project is a graphics and physics experiment, not an astronomically accurate solar-system model or a general-relativity solver.

(https://github.com/MarshyKid/Solar-System-Simulation/issues/1#issue-4809668843)

## Features

* Real-time N-body gravitational simulation
* Pairwise gravitational force calculations
* Semi-implicit Euler integration for velocity and position updates
* GPU-based sphere ray tracing in a GLSL fragment shader
* Per-pixel diffuse lighting
* Hard shadows calculated using secondary rays
* Interactive first-person 3D camera
* Procedurally generated sphere and grid geometry
* Toggleable spacetime-curvature visualisation
* CMake-based build configuration

## Technology

* **C11**
* **OpenGL 3.3**
* **GLSL**
* **GLFW**
* **GLAD**
* **cglm**
* **CMake**

## Demo

### Ray-Traced Celestial Bodies

The celestial bodies are rendered as spheres using ray–sphere intersection calculations inside the fragment shader.

For each pixel, the shader:

1. Generates a ray from the camera.
2. Tests the ray against every sphere.
3. Selects the nearest intersection.
4. Calculates the surface normal.
5. Applies diffuse lighting from the central body.
6. Casts a secondary ray to determine whether the point is in shadow.

<!-- Add screenshot -->

![Ray-Traced Bodies](assets/ray-tracing.png)

### Spacetime-Curvature Grid

The optional grid is procedurally generated and deformed according to the positions and masses of the simulated bodies.

Each body contributes to the vertical displacement of nearby grid points, creating a visual representation of a gravitational well.

This grid is intended as an intuitive visual metaphor and does not implement Einstein's field equations.

<!-- Add screenshot -->

![Spacetime Grid](assets/spacetime-grid.png)

## Controls

| Input        | Action                     |
| ------------ | -------------------------- |
| `W`          | Move forward               |
| `S`          | Move backward              |
| `A`          | Move left                  |
| `D`          | Move right                 |
| `Space`      | Move upward                |
| `Left Shift` | Move downward              |
| Mouse        | Look around                |
| `G`          | Enable the spacetime grid  |
| `O`          | Disable the spacetime grid |
| `Esc`        | Close the application      |

## How It Works

### Physics Simulation

Each celestial body stores its:

* Position
* Velocity
* Acceleration
* Mass
* Physical radius
* Render radius
* Display colour

For every frame, the simulation first resets each body's accumulated acceleration.

It then calculates the gravitational acceleration caused by every other body using:

[
a = \frac{Gm}{r^2}
]

where:

* (G) is the scaled gravitational constant
* (m) is the mass of the attracting body
* (r) is the distance between the bodies

The acceleration contributions from all other bodies are accumulated before velocity and position are updated.

The pairwise calculation currently has a time complexity of:

[
O(n^2)
]

This is appropriate for the small number of bodies used in the demonstration.

### Numerical Integration

The simulation updates velocity before position:

```text
velocity += acceleration × deltaTime
position += velocity × deltaTime
```

This is a semi-implicit, or symplectic, Euler integration step. It generally provides better orbital stability than updating position using the previous frame's velocity.

### Rendering Pipeline

The CPU simulation and GPU renderer are separated:

```text
Celestial-body state
        ↓
N-body physics update
        ↓
Render-object transformation
        ↓
Sphere positions, radii, and colours
        ↓
GLSL shader uniforms
        ↓
Per-pixel ray tracing
```

The CPU updates the body positions, while the fragment shader handles sphere intersections, lighting, and shadows.

### Spacetime Grid Generation

The grid is generated dynamically from rows and columns of points.

For each point, the vertical displacement is calculated from the combined influence of all bodies:

```text
grid height = base height + Σ body deformation
```

The resulting points are converted into vertices and triangle indices before being rendered with OpenGL.

## Project Structure

```text
Solar-System-Simulation/
├── include/
│   ├── camera.h
│   ├── mesh.h
│   ├── physics.h
│   ├── render.h
│   ├── shader.h
│   └── shape.h
├── shaders/
│   ├── fragmentShader.glsl
│   ├── vertexShader.glsl
│   ├── gridShader.glsl
│   └── gridVertexShader.glsl
├── src/
│   ├── camera.c
│   ├── main.c
│   ├── mesh.c
│   ├── physics.c
│   ├── render.c
│   ├── shader.c
│   ├── shape.c
│   └── stb_image.c
├── external/
├── CMakeLists.txt
└── README.md
```

### Important Components

| Component             | Responsibility                                                                  |
| --------------------- | ------------------------------------------------------------------------------- |
| `main.c`              | Application setup, input handling, simulation loop, and rendering orchestration |
| `physics.c`           | Gravitational acceleration and numerical integration                            |
| `render.c`            | Transfers camera and celestial-body data to the GPU                             |
| `shape.c`             | Procedural sphere and spacetime-grid generation                                 |
| `camera.c`            | Camera position, orientation, and movement                                      |
| `fragmentShader.glsl` | Ray–sphere intersections, lighting, and shadows                                 |
| `gridShader.glsl`     | Spacetime-grid rendering                                                        |

## Building the Project

### Requirements

* macOS
* CMake 3.16 or later
* A compiler supporting C11
* OpenGL 3.3-compatible hardware

The current CMake configuration is macOS-specific and links against the included GLFW static library and Apple OpenGL frameworks.

### Build

```bash
git clone https://github.com/MarshyKid/Solar-System-Simulation.git
cd Solar-System-Simulation

mkdir build
cd build

cmake ..
cmake --build .
```

### Run

From the project root:

```bash
./build/solarSystem
```

The shader paths are currently resolved relative to the project root, so the executable should be launched from there.

## Current Limitations

* Physical values and distances are scaled for visual clarity.
* The initial bodies and their velocities are defined directly in the source code.
* The simulation does not use real astronomical ephemeris data.
* The numerical integration can accumulate error over long simulations.
* The renderer supports a fixed maximum number of sphere uniforms.
* The grid is regenerated while enabled and is not currently optimised.
* The CMake configuration currently targets macOS.
* The spacetime grid is an illustrative deformation model, not a relativistic calculation.

## Possible Improvements

* Load celestial bodies from a configuration file
* Add presets for real planetary data
* Introduce simulation speed and pause controls
* Add orbital trails
* Use fixed physics timesteps
* Compare Velocity Verlet and Runge–Kutta integration
* Move body data into shader storage buffers
* Optimise the grid using persistent GPU buffers
* Add reflections and soft shadows
* Support Linux and Windows builds
* Add automated tests for the physics calculations
* Detect and handle collisions between bodies

## What I Learned

This project was built to explore the intersection of:

* Real-time simulation
* Graphics programming
* Linear algebra
* Numerical integration
* GPU shader programming
* Procedural mesh generation
* Low-level memory and resource management in C

It also provided experience separating a CPU-based physics system from a GPU-based rendering pipeline.

## License

This project is currently provided for educational and portfolio purposes.
