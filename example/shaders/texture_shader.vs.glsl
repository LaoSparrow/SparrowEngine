#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoor;

out vec2 TexCoor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoor = aTexCoor;
}