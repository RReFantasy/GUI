//
// Created by refantasy on 2022/6/20.
//

#include "mesh.h"
#include "iostream"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : _vertices(vertices), _indices(indices)
{
    GenGLBuffers();
}

void Mesh::setColor(Vector3 color)
{
    for (auto &v : _vertices)
        v.Color = color;
}

Mesh::~Mesh()
{
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
}

bool Mesh::GenGLBuffers()
{

    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0],
                 GL_DYNAMIC_DRAW); // GL_STATIC_DRAW GL_DYNAMIC_DRAW
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_DYNAMIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Texcoord));

    glBindVertexArray(0);

    return true;
}

void Mesh::Draw(unsigned int shader_program)
{
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, _texture);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //glUseProgram(0);
}

void Mesh::UpdateGLBuffer()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

bool Mesh::LoadTexture(std::string image_path)
{
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return true;
}
