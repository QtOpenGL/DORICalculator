#version 330 core
in vec3 vertexPosition;
in vec3 vertexNormal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float ambientStrength;
uniform float lightPower;

out vec3 finalColor;

void main()
{
    // Ambient color
    vec3 ambientColor = ambientStrength * lightColor;

    // Diffuse color
    vec3 normal = normalize(vertexNormal);
    vec3 lightDirection = normalize(lightPosition - vertexPosition);
    float diffuseStrength = clamp(dot(normal,lightDirection), 0.0, 1.0);
    vec3 diffuseColor = lightPower * diffuseStrength * lightColor;

    // Specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 viewDirection = normalize(cameraPosition - vertexPosition);
    vec3 specularColor = 0.35 * pow(clamp(dot(viewDirection, reflectDirection), 0.0, 1.0), 8) * lightColor;

    finalColor = (ambientColor + diffuseColor + specularColor) * objectColor;
}
