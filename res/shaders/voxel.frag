#version 460 core

out vec4 FragColor;

in vec3 v_localPos;
in vec3 v_worldPos;
in vec3 v_normal;
flat in int v_state;

uniform vec3 u_lightColor;
uniform vec3 u_lightDir;
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

  float diffuse = max(dot(n, u_lightDir), 0.f);
  float ambient = 0.2f;

  float specularLight = 0.5f;
  vec3 viewDir = normalize(u_camPos - v_worldPos);
  vec3 halfwayDir = normalize(u_lightDir + viewDir);

  float specAmount = pow(max(dot(viewDir, halfwayDir), 0.f), 64);
  float specular = specAmount * specularLight;

  return (diffuse + ambient + specular) * u_lightColor;
}

float getAO() {
  // How close this pixel is to any edge
  float edgeX = min(v_localPos.x, 1.f - v_localPos.x);
  float edgeY = min(v_localPos.y, 1.f - v_localPos.y);
  float edgeZ = min(v_localPos.z, 1.f - v_localPos.z);

  // Tightiest edge distance
  float d = 1.f;
  if (abs(v_normal.x) > 0.5f) d = min(edgeY, edgeZ); // Use 0.5f to avoid precision bleeding
  if (abs(v_normal.y) > 0.5f) d = min(edgeX, edgeZ);
  if (abs(v_normal.z) > 0.5f) d = min(edgeX, edgeY);

  float shadowThickness = 0.05f;
  float edgeShadow = smoothstep(0.f, shadowThickness, d);
  float ao = edgeShadow * 0.5f + 0.5f;

  return ao;
}

void main() {
  vec3 baseColor = colors[v_state - 1]; // 0 is "invisible block"
  float ao = getAO();

  vec3 col = baseColor * ao * directionalLight();

  FragColor = vec4(col, 1.f);
}

