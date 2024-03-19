#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoor;

out vec3 Normal;
out vec2 TexCoor;
out vec3 FragPos;
out vec3 LightPos;

uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    TexCoor = aTexCoor;
    FragPos = vec3(view * model * vec4(aPos, 1.0f));
    LightPos = vec3(view * vec4(lightPos, 1.0f));
}