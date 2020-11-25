#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

// Each point is being transformed individually
uniform mat4 transformation_p1;
uniform mat4 transformation_p2;

void main()
{
	boolean isFirstVertex = gl_VertexID == 0;
	gl_Position = projectionMatrix * viewMatrix * (isFirstVertex ? transformation_p1 : transformation_p2) * vec4(aPosition, 1.0);
}