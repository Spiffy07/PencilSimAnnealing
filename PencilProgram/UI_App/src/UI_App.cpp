#include "UI_App.h"

#include <imgui.h>

#include "Objects.h"
#include "PenMain.h"

using namespace std::chrono_literals;

namespace MyApp {
		
		// Copied from demo
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


		// my new/altered code
	// create push button
	static Push p;
	static Push q = p;
	static std::future<void> future;		
	static std::future_status status;
	static bool buttonDisabled = false;
	static bool buttonPressed = false;

	bool show_demo_window = true;

	//table flags
	static ImGuiTableFlags flags =
		ImGuiTableFlags_ContextMenuInBody |
		ImGuiTableFlags_SizingStretchSame |
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_RowBg |
		ImGuiTableFlags_BordersV |
		ImGuiTableFlags_BordersOuter |
		ImGuiTableFlags_NoBordersInBodyUntilResize
		;

	std::array<Dealer, NUMBER_OF_DEALERS> dealers;	// TODO: figure out a way to keep this code in PenMain
													//  no dealer pointer and move semantics?
													//  also default constructor makes a list of dealers before generated is called

	void MySetup()
	{

	}

	void MyRender()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (!ImGui::Begin("PencilApp"))
		{
			ImGui::End(); // early out for collapsing
			return;
		}

		// Start of Pencil App window
			// font 
		ImGuiIO& io = ImGui::GetIO();		// TODO: Correctly upscale font. see https://github.com/ocornut/imgui/issues/1018
		const float MIN_SCALE = 0.3f;
		const float MAX_SCALE = 2.0f;
		ImGui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, MIN_SCALE, MAX_SCALE, "%.2f", ImGuiSliderFlags_AlwaysClamp); // Scale everything

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::SameLine();
		HelpMarker("Use this to show/hide the demo window for a throrough reference.");

		
		if (buttonPressed && future.wait_for(0s) != std::future_status::ready)	// TODO: wait_for() will crash if called
																				//  early exit due to 'buttonPressed'
		{
			ImGui::BeginDisabled();
			buttonDisabled = true;
		}
		else if (p != q) 
			p = q;
		if (ImGui::Button("Create Push"))
		{
			buttonPressed = true;
			future = std::async([&]() {
				do {
					q = std::move(PencilSim::PenMain(dealers));					// TODO: pushMinutes changes are kept when failed
				} while (q.fitness < PencilSim::TARGET_FITNESS_MIN || false);	// for debugging
			});
		}
		if (buttonDisabled)
		{
			ImGui::EndDisabled();
			buttonDisabled = false;
		}


			// buttons for testing flags
		//PushStyleCompact();
		//ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
		//ImGui::SameLine(); 
		//ImGui::CheckboxFlags("ImGuiTableFlags_SizingStretchSame", &flags, ImGuiTableFlags_SizingStretchSame);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ContextMenuInBody", &flags, ImGuiTableFlags_ContextMenuInBody);
		//ImGui::SameLine(); 
		//ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
		//ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
		//ImGui::SameLine(); 
		//ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter);
		//ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBodyUntilResize", &flags, ImGuiTableFlags_NoBordersInBodyUntilResize);
		//ImGui::SameLine(); 
		//HelpMarker("Using the _Resizable flag automatically enables the _BordersInnerV flag as well, this is why the resize borders are still showing when unchecking this.");
		//PopStyleCompact();
		 
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