#version 430 core
out vec4 FragColor;

in vec2 TexCoor;

uniform sampler2D texture_0;
uniform sampler2D texture_1;


void main()
{
    FragColor = mix(texture(texture_0, TexCoor), texture(texture_1, TexCoor), 0.2);
}