#include "gui_base.h"




//#include "dear_imgui/backends/imgui_impl_glfw.h"
//#include "dear_imgui/backends/imgui_impl_opengl3.h"
//#include "dear_imgui/backends/imgui_impl_opengl3_loader.h"
//#include "dear_imgui/imgui.h"

int GUIBase::InitOpenGL()
{
    /* 初始化 GLFW */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* 创建窗口 */
    _window_id = glfwCreateWindow(_width, _height, _window_title.c_str(), NULL, NULL);
    if (!_window_id)
    {
        glfwTerminate();
        return -1;
    }

    /* 创建 OpenGL 上下文 */
    glfwMakeContextCurrent(_window_id);

    /* 默认开启垂直同步 */
    glfwSwapInterval(1);

    /* 窗口缩放回调 */
    glfwSetFramebufferSizeCallback(_window_id,
                                   [](GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* 开启深度测试及超采样 */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);



    return 0;
}

void GUIBase::SetWindowTitle(std::string title)
{
    _window_title = title;
    glfwSetWindowTitle(_window_id, _window_title.c_str());
}

void GUIBase::SetWindowSize(int w, int h)
{
    glfwSetWindowSize(_window_id, w, h);
}

void GUIBase::Show()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.Fonts->AddFontFromFileTTF(std::string(std::string(GUI_SOURCE_DIR) + std::string("/consola.ttf")).c_str(), 13);
	//  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
	//   Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
	//   Enable Gamepad Controls
	//    Setup Dear ImGui style

	// ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();
	// ImGui::StyleColorsClassic();
	//   Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_window_id, true);
	ImGui_ImplOpenGL3_Init(NULL);


  /* 窗口循环 */
  while (!glfwWindowShouldClose(_window_id)) {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /* 清除缓存 */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(_bg_color[0], _bg_color[1], _bg_color[2], _bg_color[3]);

    /* 渲染 */
    BaseRender();

    // Rendering DearImgui
    ImGui::Render();
     //glViewport(0, 0, width, height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(_window_id);

    /* Poll for and process events */
    glfwPollEvents();
  }

    glfwTerminate();
}
void GUIBase::SetVsync(bool vsync)
{
    glfwSwapInterval(vsync);
}
