#ifndef ASTEROIDS_HPP_
#define ASTEROIDS_HPP_

#include <vector>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "snake.hpp"

class OpenGLWindow;

class SnakeBody {
  public:
    void initializeGL(GLuint program, int length, Snake head);
    void paintGL();
    void terminateGL();

    void update(const Snake &snake);
  
  private:
    friend OpenGLWindow;

    GLuint m_program{};
    GLint m_colorLoc{};
    GLint m_translationLoc{};
    GLint m_scaleLoc{};

    struct BodyPiece {
      GLuint m_vao{};
      GLuint m_vbo{};
      GLuint m_ebo{};

      glm::vec4 m_color{0.0f, 1.0f, 0.0f, 1.0f};
      float m_scale{0.09090909f};
      glm::vec2 m_translation{glm::vec2(0)};
    };

    std::vector<BodyPiece> m_bodypieces;

    SnakeBody::BodyPiece createPiece(glm::vec2 translation, int index);


};


#endif