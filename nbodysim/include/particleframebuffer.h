#ifndef PARTICLEFRAMEBUFFER_H
#define PARTICLEFRAMEBUFFER_H

#pragma once
#include <GL/gl3w.h>
#include <vector>
#include <iostream>
#include "particle.h"
#include "octree.h"
/**
 * @brief A buffer containing a single frame of the particle system
 * 
 */
class ParticleFrameBuffer
{
  public:
  /**
   * @brief Construct a new Particle Frame Buffer object
   * 
   * @param _particleCount the number of particles to allocate memory for
   */
    ParticleFrameBuffer(size_t _particleCount);
    /**
     * @brief Construct a new Particle Frame Buffer object
     * 
     * @param _data an const vector reference of particles to copy into the buffer
     */
    ParticleFrameBuffer(const std::vector<Particle> &_data);
    /**
     * @brief Construct a new Particle Frame Buffer object
     * 
     * @param _old the particle frame buffer to move construct from
     */
    ParticleFrameBuffer(ParticleFrameBuffer &&_old);
    ParticleFrameBuffer (const ParticleFrameBuffer&) = delete;
    ParticleFrameBuffer& operator= (const ParticleFrameBuffer&) = delete;

    ~ParticleFrameBuffer();
  /**
   * @brief return the ssbo id
   * 
   * @return GLuint 
   */
    GLuint ssbo() const;
    /**
     * @brief load the current data to the ssbo and create it if it doesn't already exist
     * 
     */
    void loadToGpu();
    /**
     * @brief unload the gpu buffer back to the system memory and free the ssbo
     * 
     */
    void unloadFromGpu();
    /**
     * @brief get the number of particles
     * 
     * @return size_t 
     */
    size_t particleCount() const;
    /**
     * @brief Add particles to the current frame, regenerating the ssbo in full to up date it
     * this could be optimized to not always resize the buffer using similar techniques to std::vector
     * 
     * @param _particles 
     */
    void addParticles(const std::vector<Particle> &_particles);
    /**
     * @brief get the particles currently in the ssbo and load them back to the system memory while not freeing the ssbo
     * 
     */
    void getFromGpu();
    /**
     * @brief Perform a simple serialization of the particle frame buffer
     * 
     * @param _stream the stream to output to
     */
    void serialize(std::ostream &_stream);
    /**
     * @brief Serialize the particle frame buffer to Houdini's Geo format to allow transfer to the program
     * 
     * @param _stream the stream to output to
     */
    void serializeToGeo(std::ostream &_stream);
    /**
     * @brief Deserialize a stream containing the result of serialize, loading it into the simulation
     * 
     * @param _stream the steam to load from
     */
    void deserialize(std::istream &_stream);

    std::vector<Particle> &particles();


  private:
  /**
   * @brief create an ssbo to store data on the gpu
   * 
   */
    void generateSSBO();
    GLuint m_ssbo = 0;
    std::vector<Particle> m_particles;
    
};

#endif