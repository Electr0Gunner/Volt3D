#include "Mesh.hpp"
#include "App.hpp"


using namespace Volt;
    
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    geometry = std::make_unique<VertexObject>();
    geometry->SetVertices(vertices);
    geometry->SetIndices(indices);
    shader = std::make_unique<Shader>();
    shader->LoadFromFile("data/shaders/default.vert", "data/shaders/default.frag");
    texture_ID = 0;
    transform = Matrix4(1.0f);  
}
    
Mesh::~Mesh()
{
    
}

void Mesh::Draw()
{
    App::Instance()->renderer->SendCommand({shader.get(), geometry.get(), transform, texture_ID, GL_TRIANGLES});
}