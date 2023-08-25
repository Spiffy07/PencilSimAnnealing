#include "UI_App.h"

#include <imgui.h>

#include "objects.h"

namespace MyApp {

	bool show_demo_window = true;
	static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;

	static void MovePush()
	{

	}

	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void MyRender()
	{
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (!ImGui::Begin("PencilApp"))
		{
			ImGui::End(); // early out for collapsing
			return;
		}

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::SameLine();
		HelpMarker("Use this to show/hide the demo window for a throrough reference.");


		ImGui::BeginTable("Push", 3, flags);
		ImGui::TableSetupColumn("Table Number");
		ImGui::TableSetupColumn("Game");
		ImGui::TableSetupColumn("Dealer");
		ImGui::TableHeadersRow();

		Push simPush; 

		for (int row = 0; row < NUMBER_OF_TABLES; row++)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < 3; column++)
			{
				ImGui::TableSetColumnIndex(column);
				switch (column) 
				{
				case (0):
					ImGui::Text("%d", row + 100);
					break;
				case (1):
					ImGui::Text("Game Name here!");
					break;
				case (2):
					ImGui::Text("Dealer Name here!");
					break;
				default:
					printf("Problem with the switch in table creation!\n");
				}
			}
		}

		ImGui::EndTable();

		ImGui::End();
	}

}