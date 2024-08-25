#ifndef SHADER_H
#define SHADER_H

typedef struct {
  GLint id;
} Shader;

[[nodiscard]]
Shader shaderCreate(const char* vsPath, const char* fsPath, const char* gsPath);

void shaderUniformVec3(const Shader* self, const char* name, const vec3 v);
void shaderUniformVec4(const Shader* self, const char* name, const vec4 v);
void shaderUniformMat4(const Shader* self, const char* name, const mat4 v);

#endif

