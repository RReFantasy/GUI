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
#include "picking_texture.h"

void MOUSE_BUTTON_CALLBACK(GLFWwindow* window, int button, int action,
	int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int g_mouse_buttons = 0, g_mouse_old_x = 0, g_mouse_old_y = 0;
float g_rotate_x = 0.0, g_rotate_y = 0.0, g_translate_z = 0.0;

std::shared_ptr<Mesh> mesh_ptr;
std::shared_ptr<GLSLShader> pick_shader;

class IRenderCallbacks
{
public:
	virtual void DrawStartCB( unsigned int ID, unsigned int DrawIndex)
	{
		glUniform1ui(glGetUniformLocation(ID, "gDrawIndex"), DrawIndex);
	}
};

class GUI3D : public GUI
{
public:
	PickingTexture pick_texture;

	void Render() override
	{
		int width, height;
		glfwGetWindowSize(_window_id, &width, &height);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		view = glm::translate(view, glm::vec3(0, 0, g_translate_z));
		view = glm::rotate(view, glm::radians(g_rotate_x), glm::vec3(1, 0, 0));
		view = glm::rotate(view, glm::radians(g_rotate_y), glm::vec3(0, 1, 0));


		
		pick_texture.EnableWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pick_shader->setMat4("projection", projection);
		pick_shader->setMat4("view", view);
		pick_shader->setMat4("model", glm::mat4(1.0));
		mesh_ptr->Draw(pick_shader->GetShaderID());
		pick_texture.DisableWriting();


		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		PickingTexture::PixelInfo Pixel = pick_texture.ReadPixel(580, 400);
		if (Pixel.PrimID != 0)
		{
			std::cout << "ok" << std::endl;
		}

		_shader.setMat4("projection", projection);
		_shader.setMat4("view", view);
		_shader.setMat4("model", glm::mat4(1.0));
        mesh_ptr->Draw(_shader.GetShaderID());
       
    }

	void  Render(IRenderCallbacks* pRenderCallbacks)
	{
		pRenderCallbacks->DrawStartCB(_shader.GetShaderID(), 1);
		// PickingPhase
	}
};

std::shared_ptr<GUI3D> gui;

int main(int argc, char* argv[])
{
	gui = std::make_shared<GUI3D>();
	gui->SetVsync(false);
	gui->pick_texture.Init(800, 600);

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
    //auto load_tex = mesh_ptr->LoadTexture(std::string(GUI_SOURCE_DIR + std::string("/src/floortile.ppm")));

	
	pick_shader = std::make_shared<GLSLShader>();
	pick_shader->BuildInPickShader();
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
