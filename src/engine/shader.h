#ifndef SHADER_H
#define SHADER_H

#include "cglm/types.h"

typedef struct {
  GLint id;
  struct { char* key; GLint value; }* uniformMap;
} Shader;

extern char* shadersFolder;

[[nodiscard]]
Shader shaderCreate(const char* vsPath, const char* fsPath, const char* gsPath);

void shaderUse(Shader* self);
void shaderSetUniform1f(Shader* self, const char* name, const float v);
void shaderSetUniform3f(Shader* self, const char* name, const vec3 v);
void shaderSetUniform4f(Shader* self, const char* name, const vec4 v);
void shaderSetUniformMat4f(Shader* self, const char* name, const mat4 v);
void shaderClear(Shader* self);

#endif

