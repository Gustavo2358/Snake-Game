#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include <fmt/core.h>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;
class SnakeBody;

class Snake{
  public:
    void initializeGL(GLuint program);
    void paintGL(const GameData &gameData);
    void terminateGL();

    void update(GameData &gameData);
  private:
    friend OpenGLWindow;
    friend SnakeBody;
    

    GLuint m_program{};
    GLint m_translationLoc{};
    GLint m_colorLoc{};
    GLint m_scaleLoc{};

    GLuint m_vao{};
    GLuint m_vbo{};
    GLuint m_ebo{};

    glm::vec4 m_color{0.0, 1.0, 0.0, 1.0};
    float m_scale{0.09090909f}; //20 x 20 grid
    glm::vec2 m_translation{glm::vec2(0)};
    
    //coordinate system
    int x = 0;
    int y = 0;
    


};


#endif