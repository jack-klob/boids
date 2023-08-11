#version 450
layout(location=0) in vec2 position;
layout(location=1) in vec2 translate;
layout(location=2) in mat2 rotation;

uniform mat4 u_proj;


void main()
{
   gl_Position = u_proj * vec4((rotation * position) + translate, 0, 1);
}