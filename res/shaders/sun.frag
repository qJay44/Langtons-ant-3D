#version 460 core

out vec4 FragColor;

in vec3 v_rayDir;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform vec3 u_camPos;
uniform float u_sunFocus;
uniform float u_sunIntensity;

void main() {
  vec3 rayDir = normalize(v_rayDir - u_camPos);

  float sun = pow(max(0.f, dot(rayDir, u_lightDir)), u_sunFocus) * u_sunIntensity;
  vec3 col = sun * u_lightColor;

  FragColor = vec4(col, 1.f);
}

