#version 330 core
in vec3 vertexPosition;
in vec3 vertexNormal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float ambientStrength;

out vec3 finalColor;

void main()
{
    // Ambient color
    vec3 ambientColor = ambientStrength * lightColor;

    // Diffuse color
    vec3 normal = normalize(vertexNormal);
    vec3 lightDirection = normalize(vertexPosition - lightPosition);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseColor = diffuseStrength * lightColor;

    finalColor = (ambientColor + diffuseColor) * objectColor;

}
