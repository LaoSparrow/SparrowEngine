#version 430 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 view_position;

in vec3 Normal;
in vec2 TexCoor;
in vec3 FragPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);

    vec3 viewDir = normalize(view_position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoor));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoor));
    vec3 emmision = vec3(texture(material.emission, TexCoor));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoor));
    FragColor = vec4(ambient + diffuse + emmision + specular, 1.0f);
}