#include "snakebody.hpp"

#include <cppitertools/itertools.hpp>

void SnakeBody::initializeGL(GLuint program){
  

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  
}

void SnakeBody::paintGL(){
    abcg::glUseProgram(m_program);

    for (const auto &piece : m_bodypieces) {
        abcg::glBindVertexArray(piece.m_vao);

        abcg::glUniform4fv(m_colorLoc, 1, &piece.m_color.r);
        abcg::glUniform1f(m_scaleLoc, piece.m_scale);
        abcg::glUniform2f(m_translationLoc, piece.m_translation.x, piece.m_translation.y);

        abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
    }

    abcg::glUseProgram(0);
}

void SnakeBody::terminateGL(){
    for (auto piece : m_bodypieces) {
        abcg::glDeleteBuffers(1, &piece.m_vbo);
        abcg::glDeleteBuffers(1, &piece.m_ebo);
        abcg::glDeleteVertexArrays(1, &piece.m_vao);

    }
}

void SnakeBody::update(const Snake &snake){
 
    
    //update m_translation using the previous part of the snake body
    int size = (int)m_bodypieces.size() - 1;
    for (int i = size; i >= 0; i-- ){
        if(i == 0){
            m_bodypieces[i].m_translation = snake.m_translation;
            m_bodypieces[i].x = snake.x;
            m_bodypieces[i].y = snake.y;
        }
           
        else{
            m_bodypieces[i].m_translation = m_bodypieces[i-1].m_translation;
            m_bodypieces[i].x = m_bodypieces[i-1].x;
            m_bodypieces[i].y = m_bodypieces[i-1].y;
        }

    }
        
}

SnakeBody::BodyPiece SnakeBody::createPiece(Snake snake, int index){

  BodyPiece bodypiece;


  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  
  if(index == 0){
      bodypiece.m_translation = glm::vec2(snake.m_translation.x, snake.m_translation.y);
      bodypiece.x = snake.x;
      bodypiece.y = snake.x;
  }
  else{
      bodypiece.m_translation = glm::vec2(m_bodypieces[index-1].m_translation.x, m_bodypieces[index-1].m_translation.y);
  }
    

  std::array vertices{glm::vec2(1.0f, 1.0f), 
                      glm::vec2(1.0f, 0.0f),
                      glm::vec2(0.0f, 0.0f),
                      glm::vec2(0.0f, 1.0f)};

  const std::array indices{0, 1, 2,
                           0, 2, 3};

  // Genereta VBO
  glGenBuffers(1, &bodypiece.m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, bodypiece.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &bodypiece.m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodypiece.m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &bodypiece.m_vao);


  // Bind vertex attributes to current VAO
  glBindVertexArray(bodypiece.m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, bodypiece.m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodypiece.m_ebo);

   // End of binding to current VAO
  glBindVertexArray(0);

  return bodypiece;
}



