#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include "Eigen/src/Core/util/Constants.h"
#include "glm/fwd.hpp"
#include "gui.h"
#include "glsl_shader.h"

#include "imgui.h"
#include "iostream"
#include "memory"
#include "mesh.h"
#include "random"
#include <memory>


void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action,
	int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int g_mouse_buttons = 0, g_mouse_old_x = 0, g_mouse_old_y = 0;
float g_rotate_x = 0.0, g_rotate_y = 0.0, g_translate_z = 0.0;

std::shared_ptr<Mesh> mesh_ptr;
std::shared_ptr<Mesh> mesh_ptr2;

class GUI3D : public GUI
{
public:
	void Render() override
	{
		int width, height;
		glfwGetWindowSize(_window_id, &width, &height);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		_shader.setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		view = glm::translate(view, glm::vec3(0, 0, g_translate_z));
		view = glm::rotate(view, glm::radians(g_rotate_x), glm::vec3(1, 0, 0));
		view = glm::rotate(view, glm::radians(g_rotate_y), glm::vec3(0, 1, 0));
		_shader.setMat4("view", view);
		_shader.setMat4("model", glm::mat4(1.0));

        mesh_ptr->Draw(_shader.GetShaderID());

		for (auto& v : mesh_ptr2->_vertices)
		{
			v.Position += Vector3{ 0, -0.001, 0 };
		}
		mesh_ptr2->UpdateGLBuffer();
		mesh_ptr2->Draw(_shader.GetShaderID());

        // dear imgui widgets
        {

            static float f = 0.0f;
            static int counter = 0;
            static bool show_demo_window = false;
            static bool show_another_window = false;
            static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                           // and append into it.

            ImGui::Text("This is some useful text."); // Display some text (you can
                                                      // use a format strings too)
            ImGui::Checkbox("Demo Window",
                            &show_demo_window); // Edit bools storing our window
                                                // open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f,
                               1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color",
                              (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets
                                         // return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.1f FPS (%.3f ms/frame)",

                        ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
            ImGui::End();
        }
    }
};

std::shared_ptr<GUI3D> gui;

int main(int argc, char* argv[])
{
	gui = std::make_shared<GUI3D>();
	gui->SetVsync(false);


	glfwSetMouseButtonCallback(gui->GetWindowID(), MOUSE_BUTTON_CALLBACK);
	glfwSetCursorPosCallback(gui->GetWindowID(), mouse_callback);
	glfwSetScrollCallback(gui->GetWindowID(), scroll_callback);

	Vertex v1;
	v1.Position = Vector3{ 0, 0, 0 };
    v1.Color = Vector3(1, 0, 0);
    v1.Texcoord = Vector2{0, 0};
    Vertex v2;
    v2.Position = Vector3(1, 0, 0);
    v2.Texcoord = Vector2{10, 0};
    Vertex v3;
    v3.Position = Vector3(0.5, 1, 0);
    v3.Texcoord = Vector2{5, 10};
    std::vector<Vertex> vertices;
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    std::vector<unsigned int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    mesh_ptr = std::make_shared<Mesh>(vertices, indices);
    auto load_tex = mesh_ptr->LoadTexture(std::string(GUI_SOURCE_DIR + std::string("/src/floortile.ppm")));

    std::vector<Vertex> vertices2 = vertices;
    for (auto &v : vertices2)
        v.Position += Vector3(0, -1, 0);
    mesh_ptr2 = std::make_shared<Mesh>(vertices2, indices);

    gui->Show();


    return 0;
}

void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action,
	int mods) {
	if ((action == GLFW_PRESS) && (button == GLFW_MOUSE_BUTTON_LEFT)) {
		g_mouse_buttons |= 1 << button;
	}
	else if ((action == GLFW_RELEASE) && (button == GLFW_MOUSE_BUTTON_LEFT)) {
		g_mouse_buttons = 0;
	}

	double pos[2];
	glfwGetCursorPos(window, &pos[0], &pos[1]);
	g_mouse_old_x = pos[0];
	g_mouse_old_y = pos[1];
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float dx, dy;
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	dx = (float)(xpos - g_mouse_old_x);
	dy = (float)(ypos - g_mouse_old_y);

	if (g_mouse_buttons & 1) {
		g_rotate_x += dy * 0.2f;
		g_rotate_y += dx * 0.2f;
	}
	else if (g_mouse_buttons & 2) {
		g_translate_z += dy * 0.01f;
	}

	g_mouse_old_x = xpos;
	g_mouse_old_y = ypos;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	gui->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

/*
  Vertex v1;
  v1.Position = glm::vec3(0, 0, 0);
  v1.Color = glm::vec3(1, 0, 0);
  Vertex v2;
  v2.Position = glm::vec3(1, 0, 0);
  Vertex v3;
  v3.Position = glm::vec3(0.5, 1, 0);
  std::vector<Vertex> vertices;
  vertices.push_back(v1);
  vertices.push_back(v2);
  vertices.push_back(v3);

  std::vector<unsigned int> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  mesh_ptr = std::make_shared<Mesh>(vertices, indices);
 */
