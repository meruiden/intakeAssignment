# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (project)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like 'build')" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

if(UNIX)
	set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -pedantic -O2 -g -pg")
	set(CMAKE_CXX_FLAGS "-Wall -Wstrict-aliasing -std=c++0x -O2")

	set(CMAKE_CXX_FLAGS "-Wall")

	# valgrind --leak-check=full ./demo
	# ./demo # creates cmon.out
	# gprof demo gmon.out > analysis.txt
	 set(CMAKE_CXX_FLAGS "-Wall -pg -std=c++11")
endif(UNIX)


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
	-std=c++11
)

# Look up SDL2 and add the include directory to our include path

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


find_package(SDL2 REQUIRED)

find_package(SDL2_mixer REQUIRED)

find_package(SDL2_ttf REQUIRED)

find_package(SDL2_image REQUIRED)

add_subdirectory(../external/Box2D {CMAKE_SOURCE_DIR}/../external/Box2D)

# Compile external dependencies

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
	${GLEW_INCLUDE_DIRS}
	${SDL2_MIXER_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	../external/Box2D/
	../external/glm/
	../csframework/
)


set(ALL_LIBS
	${OPENGL_LIBRARY}
	engine
	Box2D
	SDL2
	${SDL2_MIXER_LIBRARIES}
	${SDL2_TTF_LIBRARY}
	${SDL2IMAGE_LIBRARY}
	${GLEW_LIBRARIES}
)

set(FRAMEWORK_DIR "${CMAKE_SOURCE_DIR}/../csframework")

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS

)

# Project


add_library(engine 
	"${FRAMEWORK_DIR}/engine/camera.cpp"
	"${FRAMEWORK_DIR}/engine/camera.h"
	"${FRAMEWORK_DIR}/engine/collisionhandler.cpp"
	"${FRAMEWORK_DIR}/engine/collisionhandler.h"
	"${FRAMEWORK_DIR}/engine/color.cpp"
	"${FRAMEWORK_DIR}/engine/color.h"
	"${FRAMEWORK_DIR}/engine/config.h"
	"${FRAMEWORK_DIR}/engine/entity.cpp"
	"${FRAMEWORK_DIR}/engine/entity.h"
	"${FRAMEWORK_DIR}/engine/hudelement.cpp"
	"${FRAMEWORK_DIR}/engine/hudelement.h"
	"${FRAMEWORK_DIR}/engine/hudtext.cpp"
	"${FRAMEWORK_DIR}/engine/hudtext.h"
	"${FRAMEWORK_DIR}/engine/input.cpp"
	"${FRAMEWORK_DIR}/engine/input.h"
	"${FRAMEWORK_DIR}/engine/mesh.cpp"
	"${FRAMEWORK_DIR}/engine/mesh.h"
	"${FRAMEWORK_DIR}/engine/physicsbody.cpp"
	"${FRAMEWORK_DIR}/engine/physicsbody.h"
	"${FRAMEWORK_DIR}/engine/renderer.cpp"
	"${FRAMEWORK_DIR}/engine/renderer.h"
	"${FRAMEWORK_DIR}/engine/resourceloader.cpp"
	"${FRAMEWORK_DIR}/engine/resourceloader.h"
	"${FRAMEWORK_DIR}/engine/resourcemanager.cpp"
	"${FRAMEWORK_DIR}/engine/resourcemanager.h"
	"${FRAMEWORK_DIR}/engine/scene.cpp"
	"${FRAMEWORK_DIR}/engine/scene.h"
	"${FRAMEWORK_DIR}/engine/scenemanager.cpp"
	"${FRAMEWORK_DIR}/engine/scenemanager.h"
	"${FRAMEWORK_DIR}/engine/shader.cpp"
	"${FRAMEWORK_DIR}/engine/shader.h"
	"${FRAMEWORK_DIR}/engine/sound.cpp"
	"${FRAMEWORK_DIR}/engine/sound.h"
	"${FRAMEWORK_DIR}/engine/sprite.cpp"
	"${FRAMEWORK_DIR}/engine/sprite.h"
	"${FRAMEWORK_DIR}/engine/spritesheet.cpp"
	"${FRAMEWORK_DIR}/engine/spritesheet.h"
	"${FRAMEWORK_DIR}/engine/textmesh.cpp"
	"${FRAMEWORK_DIR}/engine/textmesh.h"
	"${FRAMEWORK_DIR}/engine/texture.cpp"
	"${FRAMEWORK_DIR}/engine/texture.h"
	"${FRAMEWORK_DIR}/engine/vector2.cpp"
	"${FRAMEWORK_DIR}/engine/vector2.h"
)

add_executable(project
	"${FRAMEWORK_DIR}/project/main.cpp"
	"${FRAMEWORK_DIR}/project/level1.cpp"
	"${FRAMEWORK_DIR}/project/level1.h"
	"${FRAMEWORK_DIR}/project/menuscene.cpp"
	"${FRAMEWORK_DIR}/project/menuscene.h"
	"${FRAMEWORK_DIR}/project/settingsbutton.cpp"
	"${FRAMEWORK_DIR}/project/settingsbutton.h"
	"${FRAMEWORK_DIR}/project/settingscene.cpp"
	"${FRAMEWORK_DIR}/project/settingscene.h"
	"${FRAMEWORK_DIR}/project/player.cpp"
	"${FRAMEWORK_DIR}/project/player.h"
)

target_link_libraries(project
	${ALL_LIBS}
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(project
		-lGL
	)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Xcode and Visual working directories
#set_target_properties(project PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/project/“)
#create_target_launcher(project WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/“)


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
#add_custom_command(
#   TARGET project POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/project${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/project/“
#)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )


# Copy assets to the build directory
file(
	COPY ../csframework/assets
	DESTINATION ${CMAKE_BINARY_DIR}
)
file(
	COPY ../csframework/shaders
	DESTINATION ${CMAKE_BINARY_DIR}
)
