#version 330 core
layout(location = 0) in vec3 vertex;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vertexPosition;

void main(void)
{
    vertexNormal = mat3(transpose(inverse(modelMatrix))) * normal;
    vertexPosition = vec3(modelMatrix * vec4(vertex,1.0));
    gl_Position = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0);
}
