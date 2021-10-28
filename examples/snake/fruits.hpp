#ifndef FRUITS_HPP_
#define FRUITS_HPP_

#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Fruits{
  public:
    void initializeGL(GLuint program);
    void paintGL(const GameData &gameData);
    void terminateGL();

    void update();
  private:
    friend OpenGLWindow;


    GLuint m_program{};
    GLint m_translationLoc{};
    GLint m_colorLoc{};
    GLint m_scaleLoc{};

    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};

    glm::vec4 m_color{1.0, 0.0, 0.0, 1.0};
    float m_scale{0.09090909f}; //20 x 20 grid
    glm::vec2 m_translation{glm::vec2(0)};

    std::default_random_engine m_randomEngine;
    std::uniform_int_distribution<int> m_randomDist{-9, 9};

    void spawnFruit();

};

#endif