#Set the project name
project(SDLGame)

#Set the CMake version
cmake_minimum_required(VERSION 3.0)

#Use Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#Import all source files
file(GLOB header_files "src/*.h" "src/*.hpp")
file(GLOB src_files "src/*.c" "src/*.cpp")

#Make a project, link in libraries from conan
ADD_EXECUTABLE(SDLGame ${header_files} ${src_files})
TARGET_LINK_LIBRARIES(SDLGame ${CONAN_LIBS})

#Set the C++ version
set(CMAKE_CXX_STANDARD 11)

#Set the project for VS to use.
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "SDLGame")

#Copy the content folder to the /build for VS to be able to find them.
FILE(COPY content/ DESTINATION content)

#Copy the content folder to the /build /bin so the exe can find them.
FILE(COPY content/ DESTINATION bin/content)

#Copy the content copy script into the /build folder, this is used by VS to copy the content across on build.
FILE(COPY "pre_build_Script.bat" DESTINATION .)

#Perform prebuild
add_custom_command(TARGET SDLGame PRE_BUILD COMMAND "pre_build_Script.bat")