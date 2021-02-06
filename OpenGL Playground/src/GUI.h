#pragma once

//ImGUI
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "Scene.h"

class GUI
{
private:
public:
	static GUI* GUI_Instance;
	
	GUI();
	~GUI();
	void StartFrame();
	void EndFrame();
	void OnUpdate();

};