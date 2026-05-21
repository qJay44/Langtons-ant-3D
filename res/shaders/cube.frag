#version 460 core

out vec4 FragColor;

in vec3 v_worldPos;
in vec3 v_normal;

uniform vec3 u_lightColor;
uniform vec3 u_lightDir;
uniform vec3 u_camPos;

vec3 directionalLight() {
  vec3 n = normalize(v_normal);

  float diffuse = max(dot(n, u_lightDir), 0.f);
  float ambient = 0.2f;

  float specularLight = 0.5f;
  vec3 viewDir = normalize(u_camPos - v_worldPos);
  vec3 halfwayDir = normalize(u_lightDir + viewDir);

  float specAmount = pow(max(dot(viewDir, halfwayDir), 0.f), 64);
  float specular = specAmount * specularLight;

  return (diffuse + ambient + specular) * u_lightColor;
}

void main() {
  vec3 col = directionalLight();
  FragColor = vec4(col, 1.f);
}

