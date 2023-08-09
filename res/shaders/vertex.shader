#version 330 core
layout(location=0) in vec2 position;
layout(location=1) in vec2 translate;

uniform mat4 u_proj;


void main()
{
   gl_Position = u_proj * vec4(position + translate, 0, 1);
}