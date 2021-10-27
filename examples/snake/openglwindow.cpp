#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gsl/gsl>



void OpenGLWindow::handleEvent(SDL_Event &event) {
  
  
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w){
      m_gameData.m_input.reset();
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    }
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
      m_gameData.m_input.reset();
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    }
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a){
      m_gameData.m_input.reset();
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    } 
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d){
      m_gameData.m_input.reset();
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    }  
  }
}

void OpenGLWindow::initializeGL() {
  // Enable Z-buffer test
  glEnable(GL_DEPTH_TEST);

  // Create shader program
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "UnlitVertexColor.vert",
                                    getAssetsPath() + "UnlitVertexColor.frag");

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_snake.initializeGL(m_objectsProgram);
  m_snakebody.initializeGL(m_objectsProgram, 8, m_snake);
  m_gameData.m_input.set(static_cast<size_t>(Input::Up));
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

}

void OpenGLWindow::paintUI() {
  // Parent class will show fullscreen button and FPS meter
  abcg::OpenGLWindow::paintUI();

  // Our own ImGui widgets go below
  {
    
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
}
