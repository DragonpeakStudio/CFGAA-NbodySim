# CFGAA - Rowan Cruse Howse - Assignment - N-Body particle sim
## Contact Info
email: s5408935@bournemouth.ac.uk

## Overview
A GPU accelerated N-Body particle system for simulating gravitational effects, such as planet formation or asteroid collision. 
The program uses OpenGL, via NGL, for compute shader based processing and particle rendering as well as QT6 for the user interface.
As a basis for implementing the N-Body algorithm I will be using: https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-31-fast-n-body-simulation-cuda
Although I will be using GLSL compute shaders not CUDA the same algorithm can be implemented.
Initially I will implement the simple All Pairs algorithm, essentially just looping through all particles for each particle and summing forces (N^2 complexity). If time allows I might also look into some form of approximation for distant particles to avoid looping though them all, the above chapter briefly discusses hierarchical approaches.

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
### Building
cd programming-project-Rowan-Cruse-Howse  
mkdir build  
cd build  
cmake ..  
make  

### Running
./NBodyUI  
Requires a powerful GPU, Tested on a 2080 Super Max-Q w/ Ubuntu 22.04 & Qt 6.2.

### Usage
UI is currently incomplete, so below instructions might change.  
File Menu(Top Left):  
Save, saves the simulation to a file. This currently uses a ascii based system and produces very large files, 240 frames of the default sim is ~400mb.
Hopefully this can be improved.  

Load, Loads the files created by the above, currently has some issues, will not properly display until the sim has been played back atlease once and can sometimes create broken particles.  

Export .geo, UNIMLEMENTED, will export .geo files for Houdini.  

Viewport:  
Hold LMB - Orbit  
Hold LMB + Shift - Pan  
Hold LMB + Ctrl - Zoom  
Mouse Wheel - Zoom  

Controls:  
Reset Sim, Clears the simulation to blank, as controls for adding new particles do not yet exist this should probably not be used execept for testing.  

Playback:  
Currently no labels, so from left to right:  
Current frame number.  
Frame Rate, effects simulation FPS and playback FPS, might make these seperate controls in the future  
ToStart, Rewind, Pause, Play, ToEnd - playback controls for simulation, note that on a long sim ToEnd might take a while as it has to calc the sim up to that point.  
Frame Slider, Slider to control the current frame.  
Frame End, End of the simulation, can increase or reduce to manipulate simulation length.  
