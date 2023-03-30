#version 430 core
struct Particle
{
  vec4 position;
  vec3 velocity;
  float radius;
  vec3 colour;
  float mass;
};

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uv;
layout(std430, binding = 0) buffer particles {

  Particle particle[];
};

out vec3 colour;

uniform mat4 viewProj;

void main()
{
  gl_Position = viewProj*vec4(pos*particle[gl_InstanceID].radius+particle[gl_InstanceID].position.xyz, 1.);
  colour = particle[gl_InstanceID].colour;
}