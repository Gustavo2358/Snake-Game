#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gsl/gsl>



void OpenGLWindow::handleEvent(SDL_Event &event) {
  
  if(m_gameData.m_state == State::Playing){
    // Keyboard events
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w){
        if (!m_gameData.m_input[static_cast<size_t>(Input::Down)]){
          m_gameData.m_input.reset();
          m_gameData.m_input.set(static_cast<size_t>(Input::Up));
        }
      }
      if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
        if (!m_gameData.m_input[static_cast<size_t>(Input::Up)]){
          m_gameData.m_input.reset(); 
          m_gameData.m_input.set(static_cast<size_t>(Input::Down));
        }
        
      }
      if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a){
        if (!m_gameData.m_input[static_cast<size_t>(Input::Right)]){
          m_gameData.m_input.reset();
          m_gameData.m_input.set(static_cast<size_t>(Input::Left));
        }
      } 
      if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d){
        if (!m_gameData.m_input[static_cast<size_t>(Input::Left)]){
          m_gameData.m_input.reset();
          m_gameData.m_input.set(static_cast<size_t>(Input::Right));
        }
      }  
    }
  }
}

void OpenGLWindow::initializeGL() {
   // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }
  
  // Enable Z-buffer test
  glEnable(GL_DEPTH_TEST);

  // Create shader program
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "UnlitVertexColor.vert",
                                    getAssetsPath() + "UnlitVertexColor.frag");

  restart();
}

void OpenGLWindow::restart() {


  m_snake.initializeGL(m_objectsProgram);
  m_snakebody.initializeGL(m_objectsProgram);
  m_fruits.initializeGL(m_objectsProgram);
  
  m_gameData.m_input.set(static_cast<size_t>(Input::Up));
  m_snakebody.m_bodypieces.clear();
  m_snakebody.m_length = 0;
  m_gameData.m_state = State::Playing;
}

void OpenGLWindow::update(){
  

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  
  if(m_elapsedTimer.elapsed() < 0.1f) return; //simulate low frame rate
  m_elapsedTimer.restart();
  
  m_snakebody.update(m_snake);
  m_snake.update(m_gameData);
  
  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
  }
  
  //CRIAR UMA SISTEMA DE COORDENADAS PARA A CABEÇA E CADA PARTE DO CORPO
  

}

void OpenGLWindow::paintGL() {
  
    
    
  update();

  // Set the clear color
  glClearColor(0, 0, 0, 1);
  // Clear the color buffer and Z-buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Adjust viewport
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Check whether to render the next polygon
  //if (m_elapsedTimer.elapsed() < m_delay / 1000.0) return;
  //m_elapsedTimer.restart();
    
  m_snake.paintGL(m_gameData);
  m_snakebody.paintGL();
  m_fruits.paintGL(m_gameData);

}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();

  {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);

}

void OpenGLWindow::terminateGL() {
  // Release OpenGL resources
  glDeleteProgram(m_objectsProgram);
  
  m_snake.terminateGL();
  m_snakebody.terminateGL();
  m_fruits.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Check collision between snake and fruits

  const auto fruitPosition{m_fruits.m_translation};
  const auto distanceFruit{
    glm::distance(m_snake.m_translation, fruitPosition)};

  if (distanceFruit < m_snake.m_scale * 0.1f + m_fruits.m_scale * 0.1f) {
    m_fruits.spawnFruit();
    m_snakebody.m_bodypieces.push_back(m_snakebody.createPiece(m_snake, m_snakebody.m_length));
    m_snakebody.m_length++;

  // Check collision between snake and it's own body
  }  
  if(m_snakebody.m_length >= 1){

    //DEBUG COORDINATES  
    // fmt::print("Head:({}, {})\n", m_snake.x, m_snake.y);
    // for (auto piece : m_snakebody.m_bodypieces){
    //   fmt::print("body: ({}, {})\n", piece.x, piece.y);
    // }
      
    for (int i = 1; i < (int)m_snakebody.m_bodypieces.size(); i++) { 
      
      if (m_snake.x == m_snakebody.m_bodypieces[i].x && m_snake.y == m_snakebody.m_bodypieces[i].y ) {
        fmt::print("GAME OVER!!!");
        m_gameData.m_state = State::GameOver;
        m_gameData.m_input.reset();
        m_restartWaitTimer.restart();
      }
    }
  }

}

  
