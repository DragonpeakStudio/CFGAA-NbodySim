#ifndef NBODYPARTICLESYSTEM_H
#define NBODYPARTICLESYSTEM_H

#pragma once

#include <string_view>
#include "octree.h"
#include "particleframebuffer.h"
/**
 * @brief NBody particle system with support for GPU processing
 * 
 */
class NBodyParticleSystem
{
  public:
  /**
   * @brief Construct a new NBodyParticleSystem object
   * 
   * @param _updateProcess The shader program to use to process the particles
   */
    NBodyParticleSystem(std::string_view _updateProcess);
    ~NBodyParticleSystem();
    /**
     * @brief Process simulation steps up to a given frame
     * 
     * @param _frame the frame to process to
     * @param _delta the amount of time between frames eg, 1/24 for 24 fps
     */
    void processToFrame(size_t _frame, float _delta);
    /**
     * @brief Clears all particles on the given frame, making it empty, also removes all subsequent frames as they are now invalid
     * 
     * @param _frame  the frame to clear
     */
    void clearFrame(size_t _frame);
    /**
     * @brief Add new particles to frame and remove now invalid subsequent frames
     * 
     * @param _frame the frame to add to, if it is invalid the particles will be added to the last valid frame
     * @param _particles a const reference vector of particles to add 
     */
    void addToFrame(size_t _frame, const std::vector<Particle> &_particles);
    /**
     * @brief Get a const reference to particle frame buffer _frame
     * 
     * @param _frame  the frame to return
     * @return const ParticleFrameBuffer& 
     */
    const ParticleFrameBuffer &getFrame(size_t _frame);
    /**
     * @brief remove all frames after _frame
     * 
     * @param _frame the frame which will now be the last simulation
     */
    void eraseAfterFrame(size_t _frame);
    /**
     * @brief Perform a simple serialization of all frames
     * 
     * @param _stream The stream to output to
     */
    void serialize(std::ostream &_stream);
    /**
     * @brief Deserialize a stream containing the result of serialize, loading it into the simulation
     * 
     * @param _stream the stream to load from
     */
    void deserialize(std::istream &_stream);
    /**
     * @brief Serialize the specified frame to Houdini's Geo format to allow transfer to the program
     * 
     * @param _stream the stream to output to
     * @param _frameNum the frame to serialize
     */
    void serializeToGeo(std::ostream &_stream, size_t _frameNum);
    /**
     * @brief Set the Spring Coefficient
     * 
     * @param _springCoeff 
     */
    void setSpringCoeff(float _springCoeff);
    /**
     * @brief Set the Dampening Coefficient
     * 
     * @param _dampCoeff 
     */
    void setDampCoeff(float _dampCoeff);
    /**
     * @brief get the current number of frames
     * 
     * @return size_t 
     */
    size_t currentFrameCount() const;
  private:
  /**
   * @brief Process the next frame after the current end frame
   * 
   * @param _delta the amount of time since the last frame eg, 1/24 for 24 fps
   */
    void processNextFrame(float _delta);
    std::vector<ParticleFrameBuffer> m_particleBuffers;
    std::string m_updateProcess; 
    float m_dampCoeff = .4;
    float m_springCoeff = 20.;
    Octree m_octree;

};

#endif