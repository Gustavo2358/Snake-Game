#include "fruits.hpp"


void Fruits::initializeGL(GLuint program) {
  terminateGL();

  // Start pseudo-random number generator
  

  m_program = program;

  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  
  spawnFruit();
  
}

void Fruits::spawnFruit(){
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());
  m_translation = glm::vec2(m_randomDist(m_randomEngine)*m_scale, 
                            m_randomDist(m_randomEngine)*m_scale);
  

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

void Fruits::paintGL(const GameData &gameData) {
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

void Fruits::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);

}

void Fruits::update() {
  
}


