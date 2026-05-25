#version 460 core

out vec4 FragColor;

in vec2 v_uv;

layout(binding = 0) uniform sampler2D u_texTextAtlas;

uniform vec3 u_color;

void main() {
  vec3 col = texture(u_texTextAtlas, v_uv).r * u_color;
  FragColor = vec4(col, 1.f);
}

