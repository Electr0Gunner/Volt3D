#include <iostream>
#include <SrcSmith.hpp>

int main(int argc, char* argv[]) 
{
    Smith::App app;
    app.Init();
    app.SetCursorState(Smith::CursorState::STATE_CURSOR_LOCKED);
    app.Start();
}