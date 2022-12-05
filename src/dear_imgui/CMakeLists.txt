include_directories(./backends)
AUX_SOURCE_DIRECTORY(. SRC_LIST)
AUX_SOURCE_DIRECTORY(./backends SRC_LIST)
add_library(dear_imgui STATIC ${SRC_LIST})

#add_library(dear_imgui imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp 
#./backends/imgui_impl_glfw.cpp ./backends/imgui_impl_opengl3.cpp)