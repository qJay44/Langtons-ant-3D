#version 460 core

out vec4 FragColor;

in vec2 v_uv;

layout(binding = 0) uniform sampler2D u_texBlurV;
layout(binding = 1) uniform sampler2D u_texScreenHDR;

const float exposure = 0.5f;
const float gamma = 2.2f;

void main() {
  vec3 colBlur = texture(u_texBlurV, v_uv).rgb;
  vec3 colHDR = texture(u_texScreenHDR, v_uv).rgb;
  vec3 finalCol = colBlur + colHDR;

  finalCol = vec3(1.f) - exp(-finalCol * exposure);
  finalCol = pow(finalCol, vec3(1.f / gamma));

  FragColor = vec4(finalCol, 1.f);
}

