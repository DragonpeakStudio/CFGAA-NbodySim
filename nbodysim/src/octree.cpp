#include "octree.h"
#include <algorithm>
#include <iostream>
Octree::Octree  (unsigned int _maxDepth, unsigned int _desiredParticlesPerCell, unsigned int _maxCpuDepth) : m_maxDepth(_maxDepth), m_desiredParticlesPerCell(_desiredParticlesPerCell), m_maxCpuDepth(_maxCpuDepth)
{

  m_nodes.resize(nodesAtDepth(m_maxCpuDepth+1));//allocate the max possible space so that on the gpu we don't have to worry about managing placement in the array, we can just fill the space after our current node
}

Octree::~Octree()
{

}

void Octree::generate(std::vector<Particle> &_particles)
{
  //do early steps 
  auto bounds = calcBoundsAndMass(0, _particles.size(), _particles);
  m_nodes[0] = Node{0, (GLuint)(_particles.size()), 1, 0, std::get<0>(bounds), std::get<2>(bounds), std::get<1>(bounds),1};
  processNode(0, _particles, 0);
  for(unsigned int i = 0; i < m_maxCpuDepth; i++)
  {
    processLayer(i, _particles);
  }

  //upload to gpu
  buildSSBO();

  //divide further on gpu
  
}

unsigned int Octree::nodesAtDepth(unsigned int _depth)
{
  int amtOnLastLayer = 1;
  unsigned int sum = 0;
  for(unsigned int i = 0; i <= _depth; i++)
  {
    sum+=amtOnLastLayer;
    amtOnLastLayer = amtOnLastLayer*8;

  }
  return sum;
}

void Octree::processNode(size_t _nodeOffset, std::vector<Particle> &_particles, unsigned int _depth)
{
  Node &node = m_nodes[_nodeOffset];
  if(node.m_particlesEnd-node.m_particlesStart <= m_desiredParticlesPerCell)
  {
    node.m_shouldSplit = 0;
  }
  if(_depth <= m_maxCpuDepth && node.m_shouldSplit)
  {
    divideNode(_nodeOffset, _particles, _depth);
  }
}

unsigned int Octree::sortAxis(size_t _start, size_t _end, std::vector<Particle> &_particles, short _axis, float _mid)
{
  if(_end > _particles.size())_end=_particles.size();
  std::vector<Particle> oldParticles(_end-_start);
  std::copy(_particles.begin()+_start, _particles.begin()+_end, oldParticles.begin());
  unsigned int midPoint = 0;
  size_t lowInset = _start;
  size_t highInset = _end-1;

  for(size_t i = 0; i < _end-_start; i++)
  {
    bool isLow = false; 
    switch (_axis)
    {
    case 0:
      isLow = oldParticles[i].m_position.m_x < _mid ;
      break;
    case 1:
      isLow = oldParticles[i].m_position.m_y < _mid ;
      break;
    case 2:
      isLow = oldParticles[i].m_position.m_z < _mid ;
      break;
    default:
      break;
    }
    if(isLow)
    {
      _particles[lowInset] = oldParticles[i];
      lowInset++;
      midPoint++;
    }
    else
    {
      _particles[highInset] = oldParticles[i];
      highInset--;
    }
    
  }
  return midPoint+_start;
}

void Octree::divideNode(size_t _nodeOffset,std::vector<Particle> &_particles, unsigned int _depth)
{
  Node &parent = m_nodes[_nodeOffset];
  unsigned int x = sortAxis(parent.m_particlesStart, parent.m_particlesEnd, _particles, 0, (parent.m_maxExtent.m_x-parent.m_minExtent.m_x)/2.+parent.m_minExtent.m_x);

  unsigned int ya = sortAxis(parent.m_particlesStart, x, _particles, 1, (parent.m_maxExtent.m_y-parent.m_minExtent.m_y)/2.+parent.m_minExtent.m_y);
  unsigned int yb = sortAxis(x, parent.m_particlesEnd, _particles, 1, (parent.m_maxExtent.m_y-parent.m_minExtent.m_y)/2.+parent.m_minExtent.m_y);

  unsigned int za = sortAxis(parent.m_particlesStart, ya, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);
  unsigned int zb = sortAxis(ya, x, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);

  unsigned int zc = sortAxis(x, yb, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);
  unsigned int zd = sortAxis(yb,parent.m_particlesEnd, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);
  unsigned int off = 0;
  unsigned int lastLayerStart = 0;
  unsigned int layerStart = 0;
  if(_depth != 0)
  {
    lastLayerStart = nodesAtDepth(_depth);
  }
  layerStart = nodesAtDepth(_depth+1);
  

  unsigned int relativeOffset = _nodeOffset-lastLayerStart;
  off = layerStart+relativeOffset*8;
  

  auto bm = calcBoundsAndMass(parent.m_particlesStart, za, _particles);
  m_nodes[parent.m_childrenOffset] = Node{parent.m_particlesStart, za, off, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(za, ya, _particles);
  m_nodes[parent.m_childrenOffset+1] = Node{za, ya, off+1, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(ya, zb, _particles);
  m_nodes[parent.m_childrenOffset+2] = Node{ya, zb, off+2, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(zb, x, _particles);
  m_nodes[parent.m_childrenOffset+3] = Node{zb, x, off+3, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};


  bm = calcBoundsAndMass(x, zc, _particles);
  m_nodes[parent.m_childrenOffset+4] = Node{x, zc, off+4, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(zc, yb, _particles);
  m_nodes[parent.m_childrenOffset+5] = Node{zc, yb, off+5, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(yb, zd, _particles);
  m_nodes[parent.m_childrenOffset+6] = Node{yb, zd, off+6, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(zd, parent.m_particlesEnd, _particles);
  m_nodes[parent.m_childrenOffset+7] = Node{zd, parent.m_particlesEnd, off+7, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};
  
}

void Octree::processLayer(unsigned int _depth, std::vector<Particle> &_particles)
{
  unsigned int layerStart = nodesAtDepth(_depth);
  unsigned int layerEnd = nodesAtDepth(_depth+1);
  for(unsigned int i = layerStart; i < layerEnd; i++)
  {
    processNode(i, _particles, _depth);
  }
}

std::tuple<ngl::Vec3, ngl::Vec3, float> Octree::calcBoundsAndMass(size_t _start, size_t _end, const std::vector<Particle> &_particles)
{
  if(_end > _particles.size())_end=_particles.size();
  ngl::Vec3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
  ngl::Vec3 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
  float mass = 0;
  for(size_t i = _start; i < _end; i++)
  {
    const Particle &p = _particles[i];
    min.m_x = std::min(min.m_x, p.m_position.m_x);
    min.m_y = std::min(min.m_y, p.m_position.m_y);
    min.m_z = std::min(min.m_z, p.m_position.m_z);

    max.m_x = std::max(max.m_x, p.m_position.m_x);
    max.m_y = std::max(max.m_y, p.m_position.m_y);
    max.m_z = std::max(max.m_z, p.m_position.m_z);
    mass+=p.m_mass;
  }
  return std::make_tuple(min,max,mass);
}

void Octree::buildSSBO()
{
  glGenBuffers(1, &m_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Node)*nodesAtDepth(m_maxDepth+1), nullptr, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, m_nodes.size(), m_nodes.data());
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
