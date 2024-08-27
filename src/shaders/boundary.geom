#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 5) out;

out vec3 color;

in MY_VETEX_DATA {
  vec3 vertPos;
  vec3 color;
  vec2 texCoord;
  vec3 normal;
} data_in[];

uniform mat4 cam;

void main() {
  gl_Position = cam * gl_in[0].gl_Position;
  color = data_in[0].color;
  EmitVertex();

  gl_Position = cam * gl_in[1].gl_Position;
  color = data_in[1].color;
  EmitVertex();

  gl_Position = cam * gl_in[2].gl_Position;
  color = data_in[2].color;
  EmitVertex();

  EndPrimitive();
}

