#include "UI_App.h"
#include "imgui.h"

namespace MyApp {

	void MyRender()
	{
		ImGui::Begin("Win1", nullptr);

		ImGui::ShowDemoWindow();
		ImGui::End();
	}

}