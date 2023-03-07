//
// Created by refantasy on 2022/6/20.
//

#ifndef GUI_MESH_H
#define GUI_MESH_H

#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include "Eigen/src/Core/util/Constants.h"
#include <string>
#include <vector>

using Vector2 = Eigen::Matrix<float, 2, 1, 0, 2, 1>;
using Vector3 = Eigen::Matrix<float, 3, 1, 0, 3, 1>;

struct Vertex
{
    // position
    Vector3 Position{0, 0, 0};

    // normal
    Vector3 Normal{0, 0, 0};

    // color
    Vector3 Color{1, 1, 1};

    // texture coord
    Vector2 Texcoord{0, 0};
};

class Mesh
{
  public:
    explicit Mesh(std::vector<Vertex> vertices = std::vector<Vertex>{},
                  std::vector<unsigned int> indices = std::vector<unsigned int>{});

    virtual ~Mesh();

    bool LoadTexture(std::string image_path);

    void setColor(Vector3 color);

    void UpdateGLBuffer();

    virtual void Draw(unsigned int);

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;

  private:
    bool GenGLBuffers();

  protected:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int _texture = 0;

    // 深度缓冲
    unsigned int depthMapFBO;  // 帧缓冲
    unsigned int depthMap;
	
};

#endif // GUI_MESH_H
