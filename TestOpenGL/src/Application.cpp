#include "ImGui/ImGuiLayer.h"
#include "OpenGL/OpenGLInfo.h"

#include "Test.h"
#include "TestClearColor.h"
#include "TestTriangle.h"
#include "Texture2D.h"
#include "ShipGame.h"



int main()
{
    // Init OpenGL

    OpenGLInfo openglInfo;
    openglInfo.InitOpenGL();
    openglInfo.EnableBlend();

    // Init ImGui
    ImGuiLayer imguiLayer;
    imguiLayer.InitImGui(openglInfo.GetWindow());

    Test* currentMenu = nullptr;
    TestMenu* testMenu = new TestMenu(currentMenu);
    testMenu->RegisterTest<TestClearColor>("Clear Color");
    testMenu->RegisterTest<TestTriangle>("Triangle");
    testMenu->RegisterTest<Texture2D>("Texture2D");
    testMenu->RegisterTest<ShipGame>("ShipGame");

    currentMenu = testMenu;


    while (!openglInfo.ShowWindow())
    {
        openglInfo.ClearColor();

        imguiLayer.OnImGuiBegin();

        if (currentMenu)
        {
            imguiLayer.ImGuiBegin("Scene");
            currentMenu->OnUpdate();
            currentMenu->OnRender();
            imguiLayer.ImGuiEnd();
            imguiLayer.ImGuiBegin("Test");
            if (currentMenu != testMenu && imguiLayer.ImGuiButton("<-"))
            {
                delete currentMenu;
                currentMenu = testMenu;
            }

            currentMenu->OnImguiRender();
            imguiLayer.ImGuiEnd();

        }

        imguiLayer.OnImGuiEnd();

        openglInfo.PollEvent();
    }

    openglInfo.DestroyOpenGL();
    return 0;
}