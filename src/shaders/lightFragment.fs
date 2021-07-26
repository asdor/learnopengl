#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shiness;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diffuseCoef = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diffuseCoef * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularCoef = pow(max(dot(viewDir, reflectDir), 0.0), material.shiness);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * specularCoef * light.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
