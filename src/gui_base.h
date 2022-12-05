/*
 * 窗口界面基类
 * 主要处理窗口对象的创建和相关属性的设置
 *
 */

#ifndef __GUI_BASE_H__
#define __GUI_BASE_H__

//define GL_ARB_vertex_attrib_64bit
//glEnable(GL_ARB_gpu_shader_fp64); 

#include "glm/glm.hpp"

#ifndef INC_GLAD
#define INC_GLAD
#include "glad/glad.h"
#endif

#include "GLFW/glfw3.h"
#include "dear_imgui/imgui.h"
#include "dear_imgui/backends/imgui_impl_glfw.h"
#include "dear_imgui/backends/imgui_impl_opengl3.h"

#include <iostream>
#include <memory>
#include <string>

class GLFWwindow;

/* 窗口默认宽度 */
#define WIDTH_DEFAULT 800
/* 默认窗口高度 */
#define HEIGHT_DEFAULT 600
/* 窗口默认名称 */
#define WIN_NAME_DEFAULT "OpenGL Window"
/* 窗口默认背景颜色 */
#define WIN_BG_COLOR_DEFAULT                                                                                           \
    {                                                                                                                  \
        0.067f, 0.184f, 0.255f, 1.0f                                                                                   \
    }

class GUIBase
{
  public:
    /**
     * 默认构造函数
     * @param width 窗口宽
     * @param height 窗口高
     * @param title 标题
     * @param bg_color 背景颜色
     */
    explicit GUIBase(int width = WIDTH_DEFAULT, int height = HEIGHT_DEFAULT, std::string title = WIN_NAME_DEFAULT,
                     glm::vec4 bg_color = WIN_BG_COLOR_DEFAULT)
        : _width(width), _height(height), _window_title(std::move(title)), _bg_color(bg_color)
    {
        // 初始化OpenGL环境
        InitOpenGL();
    }

    /**
     * 默认析构函数
     */
    virtual ~GUIBase() = default;

    /**
     * 垂直同步设置
     * @param vsync 如果为 true ，同步，否则不同步
     */
    static void SetVsync(bool vsync = true);

    void SetWindowTitle(std::string title);
    void SetWindowSize(int w, int h);

    /**
     * 设置窗口背景颜色
     * @param bg_color RGB颜色
     * @param alpha 透明度
     */
    void SetBackGroundColor(glm::vec3 bg_color, float alpha = 1.0)
    {
        _bg_color = glm::vec4(bg_color, alpha);
    }

    /**
     * 显示窗口
     * 循环调用 Render() 进行渲染
     */
    void Show();

    GLFWwindow *GetWindowID() const
    {
        return _window_id;
    }

  protected:
    /**
     * 纯虚函数，重载执行渲染任务
     */
    virtual void BaseRender(){};

  protected:
    /* 窗口ID */
    GLFWwindow *_window_id = nullptr;

    /* 窗口宽高 */
    int _width = WIDTH_DEFAULT;
    int _height = HEIGHT_DEFAULT;

    /* 窗口标题 */
    std::string _window_title = std::string("OpenGL Window");

    /* 窗口背景颜色 */
    glm::vec4 _bg_color{};

  private:
    /**
     * 初始化 OpenGL 相关环境变量
     * @return 成功返回 0，否则返回 -1
     */
    int InitOpenGL();
};

#endif // __GUI_BASE_H__
