#version 460

out vec3 v_rayDir;

const vec2 vertices[] = vec2[](
  vec2(-1, -1),
  vec2(-1,  1),
  vec2( 1,  1),
  vec2( 1,  1),
  vec2( 1, -1),
  vec2(-1, -1)
);

uniform mat4 u_camInvPV;

void main() {
  vec2 ndc = vertices[gl_VertexID];
  vec4 worldPos = u_camInvPV * vec4(ndc, 1.f, 1.f);
  v_rayDir = worldPos.xyz / worldPos.w;

  gl_Position = vec4(ndc, 0.f, 1.f);
}

