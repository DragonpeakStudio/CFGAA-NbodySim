#include "octree.h"

Octree::Octree(unsigned int _maxDepth, unsigned int _desiredParticlesPerCell, unsigned int _maxCpuDepth) : m_maxDepth(_maxDepth), m_desiredParticlesPerCell(_desiredParticlesPerCell), m_maxCpuDepth(_maxCpuDepth)
{
  m_nodes.resize(8^m_maxDepth);//allocate the max possible space so that on the gpu we don't have to worry about managing placement in the array, we can just fill the space after our current node
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


  //upload to gpu

  //divide further on gpu
}

void Octree::processNode(size_t _nodeOffset, std::vector<Particle> &_particles, unsigned int _depth)
{
  Node &node = m_nodes[_nodeOffset];
  if(node.m_particlesSize <= m_desiredParticlesPerCell)
  {
    node.m_shouldSplit = 0;
  }
  if(_depth < m_maxCpuDepth && node.m_shouldSplit)
  {
    divideNode(_nodeOffset, _particles, _depth+1);

  }
}

unsigned int Octree::sortAxis(size_t _start, size_t _end, std::vector<Particle> &_particles, short _axis, float _mid)
{
  if(_end > _particles.size())_end=_particles.size();
  std::vector<Particle> oldParticles(_particles.begin()+_start, _particles.begin()+_end);
  unsigned int midPoint = 0;
  size_t lowInset = _start;
  size_t highInset = _end;

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
      isLow = oldParticles[i].m_position.m_y < _mid ;
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
  return midPoint;
}

void Octree::divideNode(size_t _nodeOffset,std::vector<Particle> &_particles, unsigned int _depth)
{
  Node &parent = m_nodes[_nodeOffset];
  unsigned int x = sortAxis(parent.m_particlesStart, parent.m_particlesStart+parent.m_particlesSize, _particles, 0, (parent.m_maxExtent.m_x-parent.m_minExtent.m_x)/2.+parent.m_minExtent.m_x);

  unsigned int ya = sortAxis(parent.m_particlesStart, parent.m_particlesStart+x, _particles, 1, (parent.m_maxExtent.m_y-parent.m_minExtent.m_y)/2.+parent.m_minExtent.m_y);
  unsigned int yb = sortAxis(parent.m_particlesStart+x, parent.m_particlesStart+parent.m_particlesSize, _particles, 1, (parent.m_maxExtent.m_y-parent.m_minExtent.m_y)/2.+parent.m_minExtent.m_y);

  unsigned int za = sortAxis(parent.m_particlesStart, parent.m_particlesStart+ya, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);
  unsigned int zb = sortAxis(parent.m_particlesStart+ya, parent.m_particlesStart+x, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);

  unsigned int zc = sortAxis(parent.m_particlesStart+x, parent.m_particlesStart+yb, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);
  unsigned int zd = sortAxis(parent.m_particlesStart+yb, parent.m_particlesStart+parent.m_particlesSize, _particles, 2, (parent.m_maxExtent.m_z-parent.m_minExtent.m_z)/2.+parent.m_minExtent.m_z);

  auto bm = calcBoundsAndMass(parent.m_particlesStart, parent.m_particlesStart+za, _particles);
  m_nodes[_nodeOffset+1*_depth*8] = Node{parent.m_particlesStart, parent.m_particlesStart+za, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(parent.m_particlesStart+za, parent.m_particlesStart+ya, _particles);
  m_nodes[_nodeOffset+2*_depth*8] = Node{parent.m_particlesStart+za, parent.m_particlesStart+ya, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(parent.m_particlesStart+ya, parent.m_particlesStart+zb, _particles);
  m_nodes[_nodeOffset+3*_depth*8] = Node{parent.m_particlesStart+ya, parent.m_particlesStart+zb, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(parent.m_particlesStart+zb, parent.m_particlesStart+x, _particles);
  m_nodes[_nodeOffset+4*_depth*8] = Node{parent.m_particlesStart+zb, parent.m_particlesStart+x, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};


  auto bm = calcBoundsAndMass(parent.m_particlesStart+x, parent.m_particlesStart+zc, _particles);
  m_nodes[_nodeOffset+5*_depth*8] = Node{parent.m_particlesStart+x, parent.m_particlesStart+zc, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(parent.m_particlesStart+zc, parent.m_particlesStart+yb, _particles);
  m_nodes[_nodeOffset+6*_depth*8] = Node{parent.m_particlesStart+zc, parent.m_particlesStart+yb, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(parent.m_particlesStart+yb, parent.m_particlesStart+zd, _particles);
  m_nodes[_nodeOffset+7*_depth*8] = Node{parent.m_particlesStart+yb, parent.m_particlesStart+zd, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};

  bm = calcBoundsAndMass(parent.m_particlesStart+zd, parent.m_particlesStart+parent.m_particlesSize, _particles);
  m_nodes[_nodeOffset+8*_depth*8] = Node{parent.m_particlesStart+zd, parent.m_particlesStart+parent.m_particlesSize, 0, (unsigned int )_nodeOffset, std::get<0>(bm), std::get<2>(bm),std::get<1>(bm), 1};
  
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
