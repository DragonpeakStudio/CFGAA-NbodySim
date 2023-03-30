#version 430 core
out vec4 FragColor;

in vec3 colour;
in vec3 normal;

uniform vec3 lightDir = vec3(0,.5,.5);
uniform vec3 lightCol = vec3(1,1,1);
uniform vec3 ambientCol = vec3(.01,.01,.01);

void main()
{
  FragColor = vec4(colour*(lightCol*dot(normal, lightDir)+ambientCol), 1);
}