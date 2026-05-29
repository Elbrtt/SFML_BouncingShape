cmake_minimum_required(VERSION 3.28)

project(SFML_Intro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
	SFML 
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 3.0.1
)
														
FetchContent_MakeAvailable(SFML)

add_executable(SFML_Bouncing_Shapes
   src/main.cpp)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(SFML_Bouncing_Shapes PRIVATE include)

target_link_libraries(SFML_Bouncing_Shapes PRIVATE
	SFML::Graphics
	SFML::Window
	SFML::System
)

