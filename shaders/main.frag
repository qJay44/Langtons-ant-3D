#version 330 core

out vec4 FragColor;

in vec3 vertPos;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

uniform vec4 lightColor;
uniform vec3 camPos;

vec4 directionalLight() {
  vec3 n = normalize(normal);
  vec3 lightDirection = normalize(vec3(1.f, 1.f, 0.f));
  float diffuse = max(dot(n, lightDirection), 0.f);
  float ambient = 0.2f;

  float specularLight = 0.5f;
  vec3 viewDirection = normalize(camPos - vertPos);
  vec3 reflectionDirection = reflect(-lightDirection, n);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.f), 16);
  float specular = specAmount * specularLight;

  return (vec4(color, 1.f) * (diffuse + ambient) + specular) * lightColor;
}

void main() {
  FragColor = directionalLight();
}

