#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "snake.hpp"
#include "snakebody.hpp"
#include "fruits.hpp"

class SnakeBody;

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  friend SnakeBody; 

  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};


  GameData m_gameData;
  SnakeBody m_snakebody;
  Snake m_snake;
  Fruits m_fruits;

  int m_delay{200};
  abcg::ElapsedTimer m_elapsedTimer;
  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void restart();
  void update();
  void checkCollisions();

  
  
};

#endif