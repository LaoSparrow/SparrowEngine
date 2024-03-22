#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoor;

out vec3 Normal;
out vec2 TexCoor;
out vec3 FragPos;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out Light viewspace_light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;
uniform Light light;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = normal_matrix * aNormal;
    TexCoor = aTexCoor;
    FragPos = vec3(view * model * vec4(aPos, 1.0f));
//    LightPos = vec3(view * vec4(lightPos, 1.0f));
    Light tmp_light = light;
    tmp_light.position = vec3(view * vec4(tmp_light.position, 1.0f));
    viewspace_light = tmp_light;
}