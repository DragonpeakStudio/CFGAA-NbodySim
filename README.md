# CFGAA - Rowan Cruse Howse - Assignment - N-Body particle sim
## Contact Info
email: s5408935@bournemouth.ac.uk

## Overview
A GPU accelerated N-Body particle system for simulating gravitational effects, such as planet formation or asteroid collision. 
The program will use OpenGL for compute shader based processing and particle rendering as well as QT6 for the user interface.
As a basis for implementing the N-Body algorithm I will be using: https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-31-fast-n-body-simulation-cuda
Although I will be using GLSL compute shaders not CUDA the same algorithm can be implemented.
## Design
The program is divided into two main parts, A library containing the functionality to process the particles and the user interface which uses the classes provided by this library.
### NBodyParticleSim Library
The NBodyParticleSim manages simulation and rendering of the particles. 
The NBodyParticleSystem class represents the particles system and provides the fuctionality for simulating the next frame.
It holds a vector of ParticleFrameBuffer's each of which represents a single frame of the simulation.
To allow simulations to be easily designed, viewed and exported, they are of a fixed length and all previous frames and kept in memory, similar to how simulations in Houdini work. This is as opposed to how they might work in games, where only two swappable buffers are stored. While that would reduce memory usage it would make it impossible to rewind and modify the simulation.
The ParticleRenderer provides a way to render the particles using instanced rendering.

### NBodyUI
The NBodyUI application is responsible for managing drawing the UI and handeling user interaction. It also uses the classes provided by NBodyParticleSim to render and simulate the particles.
RenderWidget is where the primary viewing of the simulation is performed, with it managing rendering and camera controls.
AppWindow is the main window, which uses a QMainWindow as to allow dockable widgets and easy usage of menus, such as the file menu, which allows saving and loading of the simulation as well as Houdini .geo file exports
ControlBar represents the main set of controls available to the user for manipulating the simulation, such as adding new particles and changing simulation settings.
FrameSlider allows the user to adjust the current frame, max frames and fps of the simulation as well as allowing them it initialize playback.

## Instructions
