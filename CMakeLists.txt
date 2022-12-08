cmake_minimum_required(VERSION 3.0)
project(GUI)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

add_definitions(-DGUI_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

# depend
set(GLM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/third_party/GLM/")
set(EIGEN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/third_party/eigen-3.4.0/")

# OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
link_libraries(${OPENGL_LIBRARIES})


include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/dear_imgui)
include_directories(${GLM_INCLUDE_DIR})
include_directories(${EIGEN_INCLUDE_DIR})


IF (WIN32)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
	link_directories(${PROJECT_BINARY_DIR}/lib)
    # please use vcpkg to integrate install 
	# glfw eigen 
	
ELSEIF (APPLE)
    include(FetchContent)
    FetchContent_Declare(GLFW
            GIT_REPOSITORY    https://github.com/glfw/glfw.git
            GIT_TAG           7482de6071d21db77a7236155da44c172a7f6c9e
            )
    FetchContent_MakeAvailable(GLFW)
    link_libraries(glfw)

    #GLEW
    #include_directories("/opt/homebrew/Cellar/glew/2.2.0_1/include")
    #link_directories("/opt/homebrew/Cellar/glew/2.2.0_1/lib")
    #include_directories("/usr/local/Cellar/glew/2.2.0_1/include")
    #link_directories("/usr/local/Cellar/glew/2.2.0_1/lib")
    #link_libraries(GLEW)
ELSEIF (UNIX)
    include_directories("/usr/include/")
	include_directories("/usr/local/include/")
    link_directories("/usr/lib")
    link_directories("/usr/local/lib")
    link_directories("/usr/lib64")
    link_directories("/usr/lib/x86_64-linux-gnu/")
    link_libraries(${OPENGL_LIBRARIES} glfw ${GLSLCOOKBOOK_LIBS})
ENDIF ()

# 项目源码
add_subdirectory(src)


add_executable(gui_test main.cpp)
target_link_libraries(gui_test gui)



