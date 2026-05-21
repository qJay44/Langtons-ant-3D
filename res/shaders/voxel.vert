#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inInstancePos;
layout (location = 3) in int inInstanceState;

out vec3 v_localPos;
out vec3 v_worldPos;
out vec3 v_normal;
flat out int v_state;

uniform mat4 u_camPV;

void main() {
  vec4 worldPos = vec4(inPos + inInstancePos * 2.f, 1.f);
  v_localPos = inPos * 0.5f + 0.5f;
  v_worldPos = worldPos.xyz;
  v_normal = inNormal;
  v_state = inInstanceState;
  gl_Position = u_camPV * worldPos;
}

