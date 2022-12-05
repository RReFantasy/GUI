//
// Created by refantasy on 2022/6/20.
//

#include "gui.h"
#include "GLFW/glfw3.h"
#include <vector>

float deltaTime = 0;
void GUI::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void GUI::BaseRender()
{

    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    static float lastFrame = 0;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(_window_id);

    /* 获取当前窗口宽和高*/
    int width, height;
    glfwGetWindowSize(_window_id, &width, &height);


    auto projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
    _shader.setMat4("projection", projection);
    auto view = camera.GetViewMatrix();
    _shader.setMat4("view", view);
    _shader.setMat4("model", glm::mat4(1));


    /* 用户渲染 */
    Render();
}
