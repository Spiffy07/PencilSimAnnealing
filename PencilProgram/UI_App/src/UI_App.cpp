#include "UI_App.h"

#include <imgui.h>

#include "objects.h"
#include "PenMain.h"

namespace MyApp {

	bool show_demo_window = true;
	//static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;
	static Push p;
	std::array<Dealer, NUMBER_OF_DEALERS> dealers;	// TODO: figure out a way to keep this code in PenMain
													//  no dealer pointer and move semantics?
													//  also default constructor makes a list of dealers before generated is called
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

	static void PushStyleCompact()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
	}

	static void PopStyleCompact()
	{
		ImGui::PopStyleVar(2);
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

		if (ImGui::Button("Create Push"))
		{
			do {
				p = std::move(PencilSim::PenMain(dealers));
			} while (p.fitness < PencilSim::MAX_FITNESS_POSSIBLE);
		}
		PushStyleCompact();
		ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
		ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
		ImGui::SameLine(); HelpMarker("Using the _Resizable flag automatically enables the _BordersInnerV flag as well, this is why the resize borders are still showing when unchecking this.");
		PopStyleCompact();


		ImGui::BeginTable("Push", 3, flags);
		ImGui::TableSetupColumn("Table Number");
		ImGui::TableSetupColumn("Game");
		ImGui::TableSetupColumn("Dealer");
		ImGui::TableHeadersRow();

		// TODO: create default push values for initial display or dont show table
		for (int row = 0; row < NUMBER_OF_TABLES; row++)
		{
			std::string name;
			if (p.push[row].aDealerPtr != nullptr)
				name = p.push[row].GetDealerName();

			ImGui::TableNextRow();
			for (int column = 0; column < 3; column++)
			{
				ImGui::TableSetColumnIndex(column);		
				switch (column) 
				{
				case (0):
					ImGui::Text("%d", p.push[row].GetTableNumber());
					break;
				case (1):
					ImGui::Text(p.push[row].GetTableGameName());
					break;
				case (2):
					if (!name.empty())
						ImGui::Text(p.push[row].GetDealerName().c_str());
					else
						ImGui::Text("Invalid Dealer name");
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