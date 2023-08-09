#version 330 core
layout(location=0) in vec2 position;

uniform mat4 u_proj;


void main()
{
   gl_Position = u_proj * vec4(position, 1, 1);
}