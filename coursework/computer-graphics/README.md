# Computer Graphics (CMPS 3480)

X11 rasterization and ray-tracing labs building up to a reflective-sphere final project. Split out of the old `graphics/` folder (the CMPS 3350 labs that were mixed in now live in `../software-development/`). **Languages:** C++, C (X11 / OpenGL).

| Folder | What it is |
|--------|------------|
| `lab01-image-dither` | X11 framework that loads a PPM, applies grayscale / threshold / error dithering and 90/180/270 rotation, and draws point/rect/circle/text primitives. |
| `lab02-line-drawing` | Interactive X11 line drawing — connects clicked points with a toggleable Bresenham rasterizer vs. straight lines. |
| `lab03-area-fill` | Rasterized area filling: disk, ring, rectangle, triangle (half-space & odd-even), Monte-Carlo π, checkerboard, barycentric gradient and texture-mapped triangle. |
| `lab04-rope-physics` | Double-buffered spring/rope physics — springs under gravity colliding with a movable sphere (a cloth-sim precursor). |
| `lab05-ortho-raytracer` | Simple orthographic ray tracer for spheres, planes, disks, and rings. |
| `lab06-raytracer-checker` | Orthographic ray tracer extended with checkerboard surface shading and multiple fill modes. |
| `lab07-perspective-raytracer` | Perspective ray tracer adding cylinder and cone primitives. |
| `lab08-opengl-rotation` | Intro OpenGL via GLX — circle drawing and 2×2 rotation-matrix transforms with bitmap fonts. |
| `lab09-opengl-texture` | OpenGL texture mapping — loads a JPEG (converted to PPM) into a GL texture and maps it onto geometry. |
| `lab10-raytracer-reflection-shadows` | Perspective ray tracer with mirror reflection rays and hard shadows. |
| `lab10-raytracer-capture` | A second lab10 variant (folder `a`): reflection ray tracer that captures rendered frames to disk as PPM. |
| `lab11-perlin-noise` | Fills the screen with Perlin noise — plain, turbulence, and multi-octave modes. |
| `lab12-raytracer-animation` | Perspective ray tracer that renders an animation sequence, stitching PPM/JPG frames into `out.gif`. |
| `project-masterball-raytracer` | Final "Masterball" project — a perspective ray tracer rendering a reflective sphere over a Perlin-noise foggy background; `index.html` has the write-up. |
