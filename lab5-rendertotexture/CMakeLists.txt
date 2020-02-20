cmake_minimum_required ( VERSION 3.0.2 )

project ( lab5-rendertotexture )

# Find *all* shaders.
file(GLOB_RECURSE SHADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
)

file(GLOB ENGINE
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
# Separate filter for shaders.
source_group("Shaders" FILES ${SHADERS})

# Build and link executable.
add_executable ( ${PROJECT_NAME} ${ENGINE} ${SHADERS})
target_link_libraries ( ${PROJECT_NAME} labhelper )
config_build_output()
