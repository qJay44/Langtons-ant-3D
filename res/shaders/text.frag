#version 460 core

out vec4 FragColor;

in vec2 v_uv;

layout(binding = 0) uniform sampler2D u_texTextAtlas;

void main() {
  vec3 col = vec3(texture(u_texTextAtlas, v_uv).r);
  FragColor = vec4(col, 1.f);
}

