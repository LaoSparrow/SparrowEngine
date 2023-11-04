#version 430 core
out vec4 FragColor;

in vec2 TexCoor;

uniform sampler2D texture_0;

void main()
{
    FragColor = texture(texture_0, TexCoor);
}