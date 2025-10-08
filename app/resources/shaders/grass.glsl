//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct SpotLight {
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

uniform vec3 viewPos;
uniform Material material;
uniform SpotLight spotlight;
uniform vec3 lightColor;

void main()
{
    vec3 ambient = spotlight.ambient * texture(material.diffuse, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(spotlight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotlight.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotlight.specular * spec * texture(material.specular, TexCoords).rgb;

    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon = (spotlight.cutOff - spotlight.outerCutOff);
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    float distance = length(spotlight.position - FragPos);
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular) * lightColor;
    FragColor = vec4(result, 1.0);
}
