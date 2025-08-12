#include "VertexObject.hpp"

using namespace Smith;

VertexObject::VertexObject()
{
    has_uploaded = false;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

VertexObject::~VertexObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

void VertexObject::EnsureUpload()
{
    if (!has_uploaded)
        Upload();
}

void VertexObject::Upload()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    has_uploaded = true;
}

void VertexObject::SetVertices(const std::vector<Vertex>& new_vertices)
{
    has_uploaded = false;
    vertices = new_vertices;
}

void VertexObject::SetIndices(const std::vector<uint32_t>& new_indices)
{
    has_uploaded = false;
    indices = new_indices;
}


void VertexObject::Draw(GLenum primitive)
{
    EnsureUpload();
    glBindVertexArray(VAO);
    GLuint err1= glGetError();
    glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, nullptr);
    GLuint err2= glGetError();
    glBindVertexArray(0);
}