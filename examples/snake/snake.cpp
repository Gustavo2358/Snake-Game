#include "snake.hpp"


void Snake::initializeGL(GLuint program) {
  m_program = program;

  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_translation = glm::vec2(0);
  

  std::array vertices{glm::vec2(1.0f, 1.0f), 
                      glm::vec2(1.0f, 0.0f),
                      glm::vec2(0.0f, 0.0f),
                      glm::vec2(0.0f, 1.0f)};

  const std::array indices{0, 1, 2,
                           0, 2, 3};

  // Genereta VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);


  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

   // End of binding to current VAO
  glBindVertexArray(0);


}

void Snake::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;
  // Start using the shader program
  glUseProgram(m_program);
  // Start using the VAO
  glBindVertexArray(m_vao);

  //uniform variables
  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
  abcg::glUniform4f(m_colorLoc, m_color[0], m_color[1], m_color[2], m_color[3]);
  
  abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  // End using the VAO
  glBindVertexArray(0);
  // End using the shader program
  glUseProgram(0);

}

void Snake::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);

}

void Snake::update(const GameData &gameData, float deltaTime) {
  // Move Snake
  float rate{0.4};
  if (gameData.m_input[static_cast<size_t>(Input::Left)])
    m_translation.x = m_translation.x - rate * deltaTime;
  if (gameData.m_input[static_cast<size_t>(Input::Right)])
    m_translation.x = m_translation.x + rate * deltaTime;
  if (gameData.m_input[static_cast<size_t>(Input::Up)])
    m_translation.y = m_translation.y + rate * deltaTime;
  if (gameData.m_input[static_cast<size_t>(Input::Down)])
  m_translation.y = m_translation.y - rate * deltaTime;

}
