#version 460 core

out vec4 FragColor;

in vec3 v_localPos;
in vec3 v_worldPos;
in vec3 v_normal;
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

// Gamma correction?
void main() {
  vec3 baseColor = colors[v_state - 1]; // 0 is "invisible block"

  vec3 normal = normalize(v_normal);
  vec3 viewDir = normalize(u_camPos - v_worldPos);
  vec3 halfwayDir = normalize(u_lightDir + viewDir);

  // Soft Diffuse (Molded Plastic feel)
  // We remap the diffuse range from [0, 1] to [0.3, 1.0] so the shadows aren't pitch black
  float diffuseFactor = max(dot(normal, u_lightDir), 0.f);
  float softDiffuse = mix(0.35f, 1.f, diffuseFactor);
  vec3 diffuseColor = baseColor * softDiffuse;

  float shininess = 64.f;
  float specFactor = pow(max(dot(normal, halfwayDir), 0.f), shininess);
  vec3 specularColor = vec3(0.5f) * specFactor;

  float fresnelFactor = pow(1.f - max(dot(normal, viewDir), 0.f), 9.f);
  vec3 reflectionColor = vec3(0.4f);
  vec3 mixedBase = mix(diffuseColor, reflectionColor, fresnelFactor);

  vec3 finalColor = specularColor + mixedBase;

  FragColor = vec4(finalColor, 1.f);
}

