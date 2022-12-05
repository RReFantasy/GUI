ADD_SUBDIRECTORY(dear_imgui)

AUX_SOURCE_DIRECTORY(. SRC_LIST)
add_library(gui STATIC ${SRC_LIST} mesh.h gui_base.h gui.h glsl_shader.h)
target_link_libraries(gui dear_imgui)

# 指定 gui 库的安装路径
file(GLOB Header *.h)
list(APPEND Header ./dear_imgui/imgui.h)
list(APPEND Header ./dear_imgui/imconfig.h)
install (TARGETS gui DESTINATION lib)
install (TARGETS dear_imgui DESTINATION lib)
install (FILES ${Header} DESTINATION include)
