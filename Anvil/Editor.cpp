#include "Editor.hpp"
#include "EditorGUI.hpp"

#include <format>

using namespace Anvil;


Editor::Editor(/* args */)
{
    title_label = std::format("Anvil Level Editor (v{})", ANVIL_VERSION);
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