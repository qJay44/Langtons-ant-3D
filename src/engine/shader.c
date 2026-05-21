#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/types.h>
#include <string.h>
#include <stb/stb_ds.h>

#include "shader.h"
#include "utils.h"


char* shadersFolder = NULL;

static void printError(const char* title, const char* file, const char* msg) {
  static const char* RED = "\033[1;31m";
  static const char* RESET = "\033[0m";

  printf("\n===== %s [%s%s%s] =====\n\n%s\n", title, RED, file, RESET, msg);
  int hlen = snprintf(NULL, 0, "===== %s [%s] =====", title, file);

  // Link error log doesn't provide newline at the end
  if (file[0] == 'x')
    printf("\n");

  for (int i = 0; i < hlen; i++)
    putchar('=');
  printf("\n\n");
}

static GLuint load(const char* relativePath, int type) {
  char fullPath[256];
  snprintf(fullPath, sizeof(fullPath), "%s/%s", shadersFolder, relativePath);

  char* shaderStr = readFile(fullPath);
  GLuint shaderId = glCreateShader(type);

  glShaderSource(shaderId, 1, (const char**)&shaderStr, NULL);

  free(shaderStr);
  return shaderId;
}

static GLint compile(const char* path, int type) {
  assert(shadersFolder);
  GLint shaderId = load(path, type);

  GLint hasCompiled;
  char infoLog[1024];

  glCompileShader(shaderId);
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &hasCompiled);

  // if GL_FALSE
  if (!hasCompiled) {
    glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
    printError("Shader compilation error", path, infoLog);
  }

  return shaderId;
}

static void link(GLuint program) {
  GLint hasLinked;
  char infoLog[1024];

  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &hasLinked);

  // if GL_FALSE
  if (!hasLinked) {
    glGetProgramInfoLog(program, 1024, NULL, infoLog);
    printError("Shader link error", "x", infoLog);
  }
}

static GLint getUniformLoc(Shader* self, const char* name) {
  ptrdiff_t idx = shgeti(self->uniformMap, name);
  GLint res = -1;

  if (idx == -1) {
    res = glGetUniformLocation(self->id, name);;
    shput(self->uniformMap, name, res);
  } else {
    assert(self->uniformMap);
    res = self->uniformMap[idx].value;
  }

  return res;
}

Shader shaderCreate(const char* vsPath, const char* fsPath, const char* gsPath) {
  Shader shader;
  shader.id = glCreateProgram();
  shader.uniformMap = NULL;

  GLint shaders[3];
  u8 idx = 0;

  shaders[idx++] = compile(vsPath, GL_VERTEX_SHADER);
  shaders[idx++] = compile(fsPath, GL_FRAGMENT_SHADER);

  if (gsPath)
    shaders[idx++] = compile(gsPath, GL_GEOMETRY_SHADER);

  for (u8 i = 0; i < idx; i++)
    glAttachShader(shader.id, shaders[i]);

  link(shader.id);

  for (u8 i = 0; i < idx; i++)
    glDeleteShader(shaders[i]);

  return shader;
}

void shaderUse(Shader* self) {
  glUseProgram(self->id);
}

void shaderSetUniform1f(Shader* self, const char* name, float v) {
  glProgramUniform1f(self->id, getUniformLoc(self, name), v);
}

void shaderSetUniform3f(Shader* self, const char* name, const vec3 v) {
  glProgramUniform3f(self->id, getUniformLoc(self, name), v[0], v[1], v[2]);
}

void shaderSetUniform4f(Shader* self, const char* name, const vec4 v) {
  glProgramUniform4f(self->id, getUniformLoc(self, name), v[0], v[1], v[2], v[3]);
}

void shaderSetUniformMat4f(Shader* self, const char* name, const mat4 v) {
  glProgramUniformMatrix4fv(self->id, getUniformLoc(self, name), 1, GL_FALSE, (const GLfloat*)v);
}

void shaderClear(Shader* self) {
  if (self->id) glDeleteProgram(self->id);
  self->id = 0;
}

