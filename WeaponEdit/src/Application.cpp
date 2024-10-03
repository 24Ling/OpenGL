#include "ImGui/ImGuiLayer.h"
#include "OpenGL/OpenGLInfo.h"
#include "WeaponEdit.h"


int main()
{
    // Init OpenGL

    OpenGLInfo openglInfo;
    openglInfo.InitOpenGL();
    openglInfo.EnableBlend();

    // Init ImGui
    ImGuiLayer imguiLayer;
    imguiLayer.InitImGui(openglInfo.GetWindow());

    Weapon weapon;

    while (!openglInfo.ShowWindow())
    {
        openglInfo.ClearColor();

        imguiLayer.OnImGuiBegin();

        weapon.OnUpdate();
        weapon.OnRender();
        weapon.OnImguiRender();

        imguiLayer.OnImGuiEnd();

        openglInfo.PollEvent();
    }

    weapon.SaveWeaponInfo();
    weapon.SaveWareHouseInfo();

    openglInfo.DestroyOpenGL();
    return 0;
}