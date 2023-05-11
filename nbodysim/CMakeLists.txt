
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")


add_library(NBodyParticleSim src/nbodyparticlesystem.cpp include/nbodyparticlesystem.h src/particleframebuffer.cpp include/particleframebuffer.h include/particle.h include/particlerenderer.h src/particlerenderer.cpp src/octree.cpp)
target_include_directories(NBodyParticleSim PUBLIC include)
target_link_libraries(NBodyParticleSim PUBLIC NGL)

find_package(GTest)

add_executable(NBodyTests tests/octreetests.cpp)
target_link_libraries(NBodyTests NBodyParticleSim)
target_link_libraries(NBodyTests GTest::gtest GTest::gtest_main)
