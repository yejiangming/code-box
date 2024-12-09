#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float movement;
    float shininess;
};

struct Light {
    // 点光源位置 聚光位置
    vec3 position;

    // 平行光方向 聚光方向
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 用于计算点光源衰减
    float constant;
    float linear;
    float quadratic;

    // 聚光内光锥和外光锥的余弦值
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;


void main() {

    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);

    // 执行光照计算

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;
    diffuse *= intensity;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;
    specular *= intensity;

    // vec3 emission = texture(material.emission, vec2(TexCoords.x+material.movement, TexCoords.y)).rgb;

    // float distance = length(light.position - FragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    vec3 result = (ambient+diffuse+specular);
    FragColor = vec4(result, 1.0);

}