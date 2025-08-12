#include "Mesh.hpp"
#include "App.hpp"
#include "ResourceManager.hpp"

using namespace Smith;
    
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    geometry = std::make_unique<VertexObject>();
    geometry->SetVertices(vertices);
    geometry->SetIndices(indices);
    shader = nullptr;
    Texture* tex = ResourceManager::GetTexture("grass.png");
    texture_ID = tex->GetID();
    transform = Matrix4(1.0f);  
}
    
Mesh::~Mesh()
{
    
}

void Mesh::Draw()
{
    App::Instance()->renderer->SendCommand({shader.get(), geometry.get(), transform, texture_ID, GL_TRIANGLES});
}