#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

out vec3 Normal;
out vec2 TexCoor;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = normal_matrix * aNormal;
    TexCoor = aTexCoor;
    FragPos = vec3(model * vec4(aPos, 1.0f));
}