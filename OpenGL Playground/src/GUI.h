#pragma once

//ImGUI
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Scene.h"

class GUI
{
private:
	void ConstructDockspaceWindow();
public:
	static GUI* GUI_Instance;
	
	GUI();
	void StartFrame();
	void EndFrame();
	void OnUpdate();
	~GUI();

};