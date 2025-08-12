#include "Renderer3D.hpp"
#include "App.hpp"
#include "Common.hpp"
#include <format>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>


using namespace Smith;

const char* FRAGMENT_SHADER = R"glsl(#version 330 core

in vec2 vTexCoord;
in vec4 vColor;

out vec4 FragColor;

uniform vec4 uColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform bool useTexture;
uniform sampler2D uTexture;

void main()
{
    vec4 baseColor = uColor * vColor;
    if (useTexture)
        baseColor *= texture(uTexture, vTexCoord);
    FragColor = baseColor;
}
)glsl";

const char* VERTEX_SHADER = R"glsl(#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec4 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);;
    vTexCoord = aTex;
    vColor = aColor;
}
)glsl";



#define MAX_VERTICES (size_t)65536 

Renderer3D::Renderer3D()
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
    DEFAULT_SHADER->LoadFromSource(VERTEX_SHADER, FRAGMENT_SHADER);

    std::vector<Vertex> vertices = {
        // Front face (+Z)
        {{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {1, 1, 1, 1}}, // 0: top-left
        {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {1, 1, 1, 1}}, // 1: top-right
        {{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1, 1, 1, 1}}, // 2: bottom-left
        {{1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1, 1, 1, 1}}, // 3: bottom-right

        // Back face (-Z)
        {{1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1, 1, 1, 1}}, // 4
        {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1, 1, 1, 1}}, // 5
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1, 1, 1, 1}}, // 6
        {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1, 1, 1, 1}}, // 7

        // Left face (-X)
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1, 1, 1, 1}}, // 8
        {{0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {1, 1, 1, 1}}, // 9
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1, 1, 1, 1}}, //10
        {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1, 1, 1, 1}}, //11

        // Right face (+X)
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {1, 1, 1, 1}}, //12
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1, 1, 1, 1}}, //13
        {{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1, 1, 1, 1}}, //14
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1, 1, 1, 1}}, //15

        // Top face (+Y)
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1, 1, 1, 1}}, //16
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1, 1, 1, 1}}, //17
        {{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}, {1, 1, 1, 1}}, //18
        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}, {1, 1, 1, 1}}, //19

        // Bottom face (-Y)
        {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {1, 1, 1, 1}}, //20
        {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1, 1, 1, 1}}, //21
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {1, 1, 1, 1}}, //22
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {1, 1, 1, 1}}, //23
    };


    std::vector<uint32_t> indices = {
        // Front face
        0, 1, 2,  1, 3, 2,

        // Back face
        4, 5, 6,  5, 7, 6,

        // Left face
        8, 9,10,  9,11,10,

        // Right face
        12,13,14, 13,15,14,

        // Top face
        16,17,18, 17,19,18,

        // Bottom face
        20,21,22, 21,23,22
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

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);


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
    glDisable(GL_DEPTH_TEST);
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

        
        if (cmd.texture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cmd.texture);
            shader->SetInt("uTexture", 0);
            shader->SetInt("useTexture", 1);
        }

        std::vector<Camera3D*> cams = App::Instance()->cameras;
        for (const auto& camera : cams)
        {
            camera->render_target.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 proj = camera->GetProjection((float)App::Instance()->window_width / (float)App::Instance()->window_height);
            shader->SetMat4("uModel", cmd.model);
            shader->SetMat4("uView", view);
            shader->SetMat4("uProjection", proj);

            if (camera->render_target.render_type == RenderType::RENDER_WIREFRAME)
            {
                glBindTexture(GL_TEXTURE_2D, 0);
                shader->SetInt("useTexture", 0);
                shader->SetVec4("uColor", Vector4(1.0f, 1.0f, 1.0f, 1.0f));
                glLineWidth(2.0f);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            cmd.vertex_data->Draw(cmd.primitive_type);
            camera->render_target.Unbind();

            if (camera->render_target.render_type == RenderType::RENDER_WIREFRAME)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glLineWidth(1.0f);
            }
        }

    }
    command_buffer.clear();
}

RenderTarget Renderer3D::CreateRenderTarget()
{
    RenderTarget target;
    target.type = TargetType::TARGET_SCREEN;
    target.viewport_rect = GetViewport(target, App::Instance()->game_width, App::Instance()->game_height); //target is placed there just for the argument, cause it wont actually do anything with it

    glGenTextures(1, &target.color_texture);
    glBindTexture(GL_TEXTURE_2D, target.color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, target.viewport_rect.width, target.viewport_rect.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &target.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, target.FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, target.color_texture, 0);

    glGenRenderbuffers(1, &target.depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, target.depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, target.viewport_rect.width, target.viewport_rect.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, target.depth_buffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return target;
}

void Renderer3D::UpdateViewport(RenderTarget& target, int logical_width, int logical_height)
{
   	if (SDL_GetCurrentThreadID() != SDL_GetThreadID(nullptr))
		return;

	int winWidth, winHeight;
	if (!SDL_GetWindowSize(window_ptr, &winWidth, &winHeight))
		return;


	float windowAspect = (float)winWidth / (float)winHeight;
	float logicalAspect = (float)logical_width / (float)logical_height;

	int vpX, vpY, vpW, vpH;

	if (windowAspect > logicalAspect) 
	{
		vpH = winHeight;
		vpW = (int)(logicalAspect * vpH);
		vpX = (winWidth - vpW) / 2;
		vpY = 0;
	} 
	else
	{
		vpW = winWidth;
		vpH = (int)(vpW / logicalAspect);
		vpX = 0;
		vpY = (winHeight - vpH) / 2;
	}

	target.viewport_rect = Rect(vpX, vpY, vpW, vpH); 
}

Rect Renderer3D::GetViewport(const RenderTarget& target, int logical_width, int logical_height)
{

	int winWidth, winHeight;
    if (target.IsOffscreen())
    {
        winWidth = target.viewport_rect.width;
        winHeight = target.viewport_rect.height;
    }
    else
    {
	    if (!SDL_GetWindowSize(window_ptr, &winWidth, &winHeight))
		    return Rect(0, 0, 0, 0);
    }

	float windowAspect = (float)winWidth / (float)winHeight;
	float logicalAspect = (float)logical_width / (float)logical_height;

	int vpX, vpY, vpW, vpH;

	if (windowAspect > logicalAspect) 
	{
		vpH = winHeight;
		vpW = (int)(logicalAspect * vpH);
		vpX = (winWidth - vpW) / 2;
		vpY = 0;
	} 
	else
	{
		vpW = winWidth;
		vpH = (int)(vpW / logicalAspect);
		vpX = 0;
		vpY = (winHeight - vpH) / 2;
	}

	return Rect(vpX, vpY, vpW, vpH); 
}

void Renderer3D::SendCommand(RenderCommand cmd)
{
    command_buffer.push_back(cmd);
}

std::string Renderer3D::GetErrorStr()
{
    GLenum error = glGetError();
    std::string error_string;

    switch (error)
    {
    case GL_NO_ERROR:
        error_string = "None";
        break;
    case GL_INVALID_OPERATION:
        error_string = "Invalid Operation";
        break;
    case GL_OUT_OF_MEMORY:
        error_string = "Out of memory";
        break;
    case GL_INVALID_VALUE:
        error_string = "Invalid value";
        break;
    case GL_INVALID_ENUM:
        error_string = "Invalid enum";
        break;
    default:
        error_string = "Unknown";
        break;
    }
    std::string final_string = std::format("OpenGL Error : ID ({}), {}", error, error_string);
    return final_string;
}