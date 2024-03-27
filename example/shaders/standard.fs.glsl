#version 430 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    int type;

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cut_off;
    float outer_cut_off;
};

uniform Material material;
uniform vec3 view_position;

in vec3 Normal;
in vec2 TexCoor;
in vec3 FragPos;

layout(std430, binding = 0) buffer LightBuffer {
    Light lights[];
};

vec3 calc_point_light(Light light) {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);

    vec3 viewDir = normalize(view_position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoor));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoor));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoor));
    return ambient + diffuse + specular;
}

void main()
{
    vec3 result;
    for (int i = 0; i < lights.length(); i++) {
        switch (lights[i].type) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            result += calc_point_light(lights[i]);
            break;
        case 3:
            break;
        case 4:
            break;
        }
    }
    vec3 emmision = vec3(texture(material.emission, TexCoor));
    FragColor = vec4(result + emmision, 1.0f);
}