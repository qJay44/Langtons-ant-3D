#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_instancePos;
layout (location = 4) in int a_instanceState;

out vec3 v_worldPos;
out vec3 v_normal;
out vec2 v_uv;
flat out int v_state;

uniform mat4 u_camPV;

void main() {
  vec4 worldPos = vec4(a_pos + a_instancePos * 2.f, 1.f);
  v_uv = a_tex;
  v_worldPos = worldPos.xyz;
  v_normal = a_normal;
  v_state = a_instanceState;
  gl_Position = u_camPV * worldPos;
}

