
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")


add_library(NBodyParticleSim src/nbodyparticlesim.cpp)
target_include_directories(NBodyParticleSim PUBLIC include)
target_link_libraries(NBodyParticleSim PUBLIC NGL)
