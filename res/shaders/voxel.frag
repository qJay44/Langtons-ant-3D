#version 460 core

out vec4 FragColor;

in vec3 v_worldPos;
in vec3 v_normal;
flat in int v_state;

uniform vec3 u_lightColor;
uniform vec3 u_camPos;

const vec3 colors[5] = vec3[] (
  vec3(1.000f, 0.745f, 0.043f), // #ffbe0b
  vec3(0.984f, 0.337f, 0.027f), // #fb5607
  vec3(1.000f, 0.000f, 0.431f), // #ff006e
  vec3(0.513f, 0.219f, 0.925f), // #8338ec
  vec3(0.227f, 0.525f, 1.000f)  // #3a86ff
);

vec3 directionalLight() {
  vec3 n = normalize(v_normal);
  vec3 lightDir = normalize(vec3(0.57735f));

  float diffuse = max(dot(n, lightDir), 0.f);
  float ambient = 0.2f;

  float specularLight = 0.5f;
  vec3 viewDir = normalize(u_camPos - v_worldPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);

  float specAmount = pow(max(dot(viewDir, halfwayDir), 0.f), 64);
  float specular = specAmount * specularLight;

  return (diffuse + ambient + specular) * u_lightColor;
}

void main() {
  vec3 col = directionalLight() * colors[v_state];
  FragColor = vec4(col, 1.f);
}

