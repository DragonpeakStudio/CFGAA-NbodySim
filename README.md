# CFGAA - Rowan Cruse Howse - Assignment - N-Body particle sim
## Contact Info
email: s5408935@bournemouth.ac.uk
![Screenshot of application](/docs/images/Screenshot%20from%202023-04-29%2009-07-51.png "Screenshot")


## Overview
A GPU accelerated N-Body particle system for simulating gravitational effects, such as planet formation or asteroid collision. 
The program uses OpenGL, via NGL, for compute shader based processing and particle rendering as well as QT6 for the user interface.
As a basis for implementing the N-Body algorithm I will be using: https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-31-fast-n-body-simulation-cuda
Although I will be using GLSL compute shaders not CUDA the same algorithm can be implemented.
Initially I will implement the simple All Pairs algorithm, essentially just looping through all particles for each particle and summing forces (N^2 complexity). If time allows I might also look into some form of approximation for distant particles to avoid looping though them all, the above chapter briefly discusses hierarchical approaches.

## Design
The program is divided into two main parts, A library containing the functionality to process the particles and the user interface which uses the classes provided by this library.
This helps to keep the different parts of the program distict and means that in the future a different user interface could be created or it could be ported to work as a plugin for any 3d software.

### NBodyParticleSim Library
The NBodyParticleSim manages simulation and rendering of the particles. 
The NBodyParticleSystem class represents the particles system and provides the fuctionality for simulating the next frame and accessing different frames.
It holds a vector of ParticleFrameBuffer's each of which represents a single frame of the simulation.
To allow simulations to be easily designed, viewed and exported, they are of a fixed length and all previous frames are kept in memory, similar to how simulations in Houdini work. This is as opposed to how they might work in games, where only two swappable buffers are stored. While that would reduce memory usage it would make it impossible to rewind and modify the simulation.
The ParticleRenderer provides a way to render the particles using simple instanced rendering.
![UML Diagram of NBodyParticleSim Library](/docs/images/NBodyParticleSim.drawio.png "Diagram")


### NBodyUI
The NBodyUI application is responsible for managing drawing the UI and handeling user interaction. It also uses the classes provided by NBodyParticleSim to render and simulate the particles.
RenderWidget is where the primary drawing of the simulation is performed, with it also managing rendering and camera controls.
AppWindow is the main window, which uses a QMainWindow as to allow dockable widgets and easy usage of menus, such as the file menu, which allows saving and loading of the simulation as well as Houdini .geo file exports
ControlBar represents the main set of controls available to the user for manipulating the simulation, such as adding new particles and changing simulation settings.
FrameSlider allows the user to adjust the current frame, max frames and fps of the playback as well as initiate playback.
![UML Diagram of NBodyUI](/docs/images/NBodyUI.drawio.png "Diagram")

## Instructions
### Building
Assumes NGL is installed in ~/NGL and vcpkg with NGL's requirements installed in ~/vcpkg and that Qt6 will be sucessfully found. If it is not already in the path it might have to be added.
An example on how Qt might need to be specified is ``` cmake -DCMAKE_PREFIX_PATH="$HOME/Qt/6.2.4/gcc_64" .. ```
```
cd programming-project-Rowan-Cruse-Howse  
mkdir build  
cd build  
cmake ..  
make  
```
### Running
```
./NBodyUI  
```
Requires a powerful GPU, Tested on a 2080 Super Max-Q w/ Ubuntu 22.04 & Qt 6.2.

### Usage 
File Menu(Top Left):  
Save, saves the simulation to a file. This currently uses a uncompressed binary based system and produces very large files, 240 frames of the default sim is ~200mb.

Load, Loads the files created by the above.  

Export .geo, allowing the result to be used in Houdini.

Viewport:  
Hold LMB - Orbit  
Hold LMB + Shift - Pan  
Hold LMB + Ctrl - Zoom  
Mouse Wheel - Zoom  

Controls:  
Sim FPS, the framerate of the simulation, this is seperate to the playback fps, allowing the sim to use smaller timesteps for higher detail or higher timesteps for faster processing.  
Damp Coefficent, Affects how "Sticky" Collisions are.  
Spring Coefficent, Affects how "Bouncy" Collisions are.  
Reset Sim, Clears the simulation to blank.  

Add Particles:
Amount, the number of particles to add.  
Poistion, the point to add particles around.  
Radius, the radius of the sphere which particles will be placed within.  
Direction, the normalized direction of the particles velocity.  
Speed, how fast the particles will be initialized to.  
Mass, The new particles mass.  
Colour, the colour of the particles.  
Size, the radius of the added particles.  
Add Particles, Adds new particles on the current frame based on the parameters above.  

Playback:  
Currently no labels, so from left to right:  
Current frame number.  
Frame Rate, affects only playback FPS.  
ToStart, Rewind, Pause, Play, ToEnd - playback controls for simulation, note that on a long sim ToEnd might take a while as it has to calc the sim up to that point. Will add proper icons in future.  
Frame Slider, Slider to control the current frame.  
Frame End, End of the simulation, can increase or reduce to manipulate simulation length.  

## Notes
Note: Uses icons from Open Iconic: https://github.com/iconic/open-iconic under MIT Licence  
See resources/icons/ICON-LICENCE for details
