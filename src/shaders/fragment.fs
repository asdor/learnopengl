#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Light
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shiness;
};

uniform vec3 viewPos;
uniform Material material;
// uniform Light light;
uniform DirLight dirLight;

#define POINT_LIGHTS_CNT 4
uniform PointLight pointLights[POINT_LIGHTS_CNT];

uniform SpotLight spotLight;

out vec4 FragColor;

vec3 calcDirLight(DirLight i_dirLight, vec3 i_normal, vec3 i_viewDir)
{
    vec3 lightDir = normalize(-i_dirLight.direction);
    float diffuseCoef = max(dot(i_normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, i_normal);
    float specularCoef = pow(max(dot(i_viewDir, reflectDir), 0.0), material.shiness);

    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diffuseCoef * i_dirLight.diffuse;
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * i_dirLight.ambient;
    vec3 specular = vec3(texture(material.specular, TexCoords)) * specularCoef * i_dirLight.specular;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight i_pointLight, vec3 i_normal, vec3 i_fragPos, vec3 i_viewDir)
{
    vec3 lightDir = normalize(i_pointLight.position - i_fragPos);
    float diffuseCoef = max(dot(i_normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, i_normal);
    float specularCoef = pow(max(dot(i_viewDir, reflectDir), 0.0), material.shiness);

    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diffuseCoef * i_pointLight.diffuse;
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * i_pointLight.ambient;
    vec3 specular = vec3(texture(material.specular, TexCoords)) * specularCoef * i_pointLight.specular;

    float distance = length(i_pointLight.position - i_fragPos);
    float attenuation = 1.0 / (i_pointLight.quadratic * distance * distance + i_pointLight.linear * distance + i_pointLight.constant);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight i_spotLight, vec3 i_normal, vec3 i_fragPos, vec3 i_viewDir)
{
    vec3 lightDir = normalize(i_spotLight.position - i_fragPos);
    float diffuseCoef = max(dot(i_normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, i_normal);
    float specularCoef = pow(max(dot(i_viewDir, reflectDir), 0.0), material.shiness);

    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diffuseCoef * i_spotLight.diffuse;
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * i_spotLight.ambient;
    vec3 specular = vec3(texture(material.specular, TexCoords)) * specularCoef * i_spotLight.specular;

    float distance = length(i_spotLight.position - i_fragPos);
    float attenuation = 1.0 / (i_spotLight.quadratic * distance * distance + i_spotLight.linear * distance + i_spotLight.constant);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // spotlight with soft edges
    float theta = dot(lightDir, normalize(-i_spotLight.direction));
    float epsilon = i_spotLight.cutOff - i_spotLight.outerCutOff;
    float intensity = clamp((theta - i_spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 output = calcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < POINT_LIGHTS_CNT; ++i)
        output += calcPointLight(pointLights[i], norm, FragPos, viewDir);

    output += calcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(output, 1.0);

    /*
    // ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diffuseCoef = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diffuseCoef * light.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularCoef = pow(max(dot(viewDir, reflectDir), 0.0), material.shiness);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * specularCoef * light.specular;

    // spotlight with soft edges
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.quadratic * distance * distance + light.linear * distance + light.constant);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
    */
}
