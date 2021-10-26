#version 410

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

out vec4 fragColor;

uniform vec2 translation;
uniform vec4 color;
uniform float scale; 

void main() {
  gl_Position = vec4(inPosition.x * scale + translation.x, inPosition.y * scale + translation.y, 0.0, 1.0);
  fragColor = color;
}