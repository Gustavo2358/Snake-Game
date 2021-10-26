#include <fmt/core.h>

#include "openglwindow.hpp"

#include <imgui.h>
#include <array>

void OpenGLWindow::initializeGL() {
  auto windowSettings{getWindowSettings()};
  fmt::print("Initial window size: {}x{}\n", windowSettings.width,
             windowSettings.height);
}

void OpenGLWindow::paintGL() {
  // Set the clear color
  abcg::glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2],
                     m_clearColor[3]);
  // Clear the color buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();

  // Our own ImGui widgets go below
  {
    // Window begin
    ImGui::Begin("Tic-Tac-Toe");

  //  // Static text
  //  auto windowSettings{getWindowSettings()};
  //  ImGui::Text("Current window size: %dx%d (in windowed mode)",
  //              windowSettings.width, windowSettings.height);

  //  // Slider from 0.0f to 1.0f
  //  static float f{};
  //  ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

  //  // ColorEdit to change the clear color
  //  ImGui::ColorEdit3("clear color", m_clearColor.data());

  //  // More static text
  //  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
  //              1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	// custom program starts here
	ImGui::SetWindowFontScale(1.8); 

	char empty[2] = {' ','\0'};
	char X[2] = {'X','\0'};
	char O[2] = {'O','\0'};

	//create the button labels
	static char* bl[9] = {empty,empty,empty,empty,empty,empty,empty,empty,empty};
	
	static char* toggle = X;
	ImGui::Text("%c turn", *toggle);

	int buttonHeight = 100;
	int buttonWidth = 100;

    if (ImGui::BeginTable("Game", 3))
	{
		ImGui::TableNextRow();
		int count = 0;
		for(int i = 0; i < 9; i++){ 
			ImGui::TableSetColumnIndex(count);
			ImGui::Button(bl[i], ImVec2(buttonHeight,buttonWidth));
			if (ImGui::IsItemClicked() and bl[i] == empty) {
				bl[i] = toggle;
				if(toggle == X) toggle = O;
				else toggle = X;
			}
			count++;
			if(count == 3){
				ImGui::TableNextRow();
				count = 0;
				
			}
			
		}

		
		ImGui::EndTable();
	}
	
	// Nx50 button, where N is the remaining width available
	ImGui::Button("Restart Game", ImVec2(-1, 50));
	// See also IsItemHovered, IsItemActive, etc
	if (ImGui::IsItemClicked()) {
		
		for(int i = 0;i < 9;i++){
			bl[i] = empty;

		}
	}
	
    // Window end
    ImGui::End();
  }
}
