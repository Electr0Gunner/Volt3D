#include "Renderer3D.hpp"
#include "App.hpp"
#include "Common.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>


using namespace Volt;

#define MAX_VERTICES (size_t)65536 

Renderer3D::Renderer3D(int test)
{
    context = nullptr;
    window_ptr = App::Instance()->window->raw_window;
}
    
Renderer3D::~Renderer3D()
{
    Cleanup();

    delete TEST_MESH;
    delete DEFAULT_SHADER;
}
    
RendererInitCode Renderer3D::Init()
{
    context = SDL_GL_CreateContext(window_ptr);

    SDL_GL_MakeCurrent(window_ptr, context);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        //TODO: ADD ERROR HANDLER
    }
    SDL_GL_SetSwapInterval(1);

    
    DEFAULT_SHADER = new Shader();
    DEFAULT_SHADER->LoadFromFile("data/shaders/default.vert", "data/shaders/default.frag");

    std::vector<Vertex> vertices = {
        {{0.0f, 1.1f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // top-left
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // top-right
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // bottom-left
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // bottom-right
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,
        2, 1, 3
    };


    TEST_MESH = new Mesh(vertices, indices);

    return CODE_RENDERER_OK;
}

void Renderer3D::Cleanup()
{
    SDL_GL_DestroyContext(context);
}

void Renderer3D::BeginFrame()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void Renderer3D::Render()
{

    TEST_MESH->Draw();

}

void Renderer3D::EndFrame()
{
    Flush();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window_ptr);
}

void Renderer3D::Flush()
{
    for (const auto& cmd: command_buffer)
    {
        Shader* shader;
        if (cmd.shader == nullptr)
            shader = DEFAULT_SHADER;
        else
            shader = cmd.shader;
        shader->Use();

        
        glm::mat4 view = App::Instance()->camera->GetViewMatrix();
        glm::mat4 proj = App::Instance()->camera->GetProjection(1600.0f/900.0f);
        shader->SetMat4("uModel", cmd.model);
        shader->SetMat4("uView", view);
        shader->SetMat4("uProjection", proj);

        if (cmd.texture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cmd.texture);
            shader->SetInt("uTexture", cmd.texture);
        }
        cmd.vertex_data->Draw(cmd.primitive_type);
    }
    command_buffer.clear();
}

void Renderer3D::SendCommand(RenderCommand cmd)
{
    command_buffer.push_back(cmd);
}

std::string Renderer3D::GetErrStr()
{
    return "ERROR: NONE";
}