#version 460 core

out vec4 FragColor;

in vec3 v_worldPos;
in vec3 v_normal;
in vec2 v_uv;
flat in int v_state;

uniform vec3 u_lightDir;
uniform vec3 u_camPos;

const vec3 colors[5] = vec3[] (
  vec3(1.f,   0.15f, 0.15f), // State 1: Toy Red
  vec3(0.15f, 0.85f, 0.15f), // State 2: Toy Green
  vec3(0.15f, 0.45f, 1.f),   // State 3: Toy Blue
  vec3(1.f,   0.8f,  0.f),   // State 4: Toy Yellow
  vec3(0.f,   1.f,   1.f)    // State 5: Toy Cyan
);

void main() {
  vec3 baseColor = colors[v_state - 1]; // 0 state is "invisible block" (v_state will never be 0)

  vec3 normal = normalize(v_normal);
  vec3 viewDir = normalize(u_camPos - v_worldPos);
  vec3 halfwayDir = normalize(u_lightDir + viewDir);

  vec2 dist = min(v_uv, vec2(1.f) - v_uv);
  float minGlow = min(dist.x, dist.y);

  vec3 finalColor = baseColor;

  if (minGlow < 0.03f)
    finalColor = baseColor * 0.5f;

  // Soft Diffuse (Molded Plastic feel)
  // We remap the diffuse range from [0, 1] to [0.3, 1.0] so the shadows aren't pitch black
  float diffuseFactor = max(dot(normal, u_lightDir), 0.f);
  float softDiffuse = mix(0.35f, 1.f, diffuseFactor);
  vec3 diffuseColor = finalColor * softDiffuse;

  float shininess = 64.f;
  float specFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
  vec3 specularColor = vec3(0.5f) * specFactor;

  finalColor = diffuseColor + specularColor;

  FragColor = vec4(finalColor, 1.f);
}

