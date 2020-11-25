#version 330 core

out vec4 FragColor;

uniform vec3 headBoxColor;
uniform float headBoxOpacity;

void main()
{
   FragColor = vec4(headBoxColor, headBoxOpacity);
}