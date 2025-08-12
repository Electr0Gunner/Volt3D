#include "Editor.hpp"
#include "EditorGUI.hpp"

using namespace Anvil;


Editor::Editor(/* args */)
{
    title_label = "Anvil Level Editor";
}
    
Editor::~Editor()
{
}

void Editor::Init()
{
    App::Init();

    imgui_manager->AddWindow(std::make_unique<EditorGUI>());
    for (const auto& camera : Smith::App::Instance()->cameras)
    {
        camera->render_target.type = Smith::TargetType::TARGET_FBO;
    }

}