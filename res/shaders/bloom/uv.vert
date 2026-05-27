#version 460 core

const vec2 vertices[] = vec2[](
  vec2(-1, -1),
  vec2(-1,  1),
  vec2( 1,  1),
  vec2( 1,  1),
  vec2( 1, -1),
  vec2(-1, -1)
);

out vec2 v_uv;

uniform mat4 u_camInvPV;
uniform vec3 u_camPos;

void main() {
  vec2 ndc = vertices[gl_VertexID];
  v_uv = ndc * 0.5f + 0.5f;

  gl_Position = vec4(ndc, 0.f, 1.f);
}

