#version 330 core

out vec4 FragColor;

uniform vec3 traceColor;
uniform float traceOpacity;

void main()
{
   FragColor = vec4(traceColor, traceOpacity);
}