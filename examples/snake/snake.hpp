#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Snake{
  public:
    void initializeGL(GLuint program);
    void paintGL(const GameData &gameData);
    void terminateGL();

    void update(const GameData &gameData, float deltaTime);
  private:
    friend OpenGLWindow;

    GLuint m_program{};
    GLint m_translationLoc{};
    GLint m_colorLoc{};
    GLint m_scaleLoc{};

    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};

    glm::vec4 m_color{0.0, 1.0, 0.0, 1.0};
    float m_scale{0.08f};
    glm::vec2 m_translation{glm::vec2(0)};
    


};


#endif