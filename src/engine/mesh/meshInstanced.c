#include "meshInstanced.h"

#include <stdio.h>

#include "GLBuffer.h"
#include "cglm/struct/mat4.h"
#include "Context.h"

static void setCameraUniforms(const Camera* cam, Shader* shader) {
  shaderSetUniform1f   (shader, "u_camNear"   , cam->near);
  shaderSetUniform1f   (shader, "u_camFar"    , cam->far);
  shaderSetUniform1f   (shader, "u_camFov"    , cam->fov);
  shaderSetUniform3f   (shader, "u_camPos"    , cam->position.raw);
  shaderSetUniform3f   (shader, "u_camUp"     , cam->up.raw);
  shaderSetUniformMat4f(shader, "u_camProj"   , cam->proj.raw);
  shaderSetUniformMat4f(shader, "u_camView"   , cam->view.raw);
  shaderSetUniformMat4f(shader, "u_camPV"     , glms_mat4_mul(cam->proj, cam->view).raw);
}

MeshInstanced meshInstancedCreatePN(const MeshData* data) {
  MeshInstanced mesh;
  mesh.indices = data->indSize / sizeof(data->indices[0]);
  printf("%u\n", mesh.indices);

  vaoGen(&mesh.vao, 1);
  GLBuffer_gen(&mesh.vbo, GL_ARRAY_BUFFER, 1);
  GLBuffer_gen(&mesh.vboInstanced, GL_ARRAY_BUFFER, 1);
  GLBuffer_gen(&mesh.ebo, GL_ELEMENT_ARRAY_BUFFER, 1);

  vaoBind(&mesh.vao);

  GLBuffer_allocate(&mesh.vbo, data->vertices, data->vertSize, GL_STATIC_DRAW);
  GLBuffer_allocate(&mesh.ebo, data->indices, data->indSize, GL_STATIC_DRAW);

  GLBuffer_bind(&mesh.vbo);
  GLBuffer_bind(&mesh.ebo);

  size_t typeSize = sizeof(float);
  size_t stride = typeSize * 3 * 2;
  vaoLinkAttrib(0, 3, GL_FLOAT, stride, (void*)(0 * typeSize));
  vaoLinkAttrib(1, 3, GL_FLOAT, stride, (void*)(3 * typeSize));

  GLBuffer_bind(&mesh.vboInstanced);
  vaoLinkAttrib(2, 3, GL_FLOAT, sizeof(InstanceVertex), (void*)0);
  glEnableVertexAttribArray(3);
  glVertexAttribIPointer(3, 1, GL_INT, sizeof(InstanceVertex), (void*)(3 * sizeof(float)));

  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);

  vaoUnbind();
  GLBuffer_unbind(&mesh.vbo);

  return mesh;
}

void meshInstancedDraw(MeshInstanced* self, const Camera* cam, Shader* shader) {
  if (self->instanceCount == 0)
    return;

  vaoBind(&self->vao);

  setCameraUniforms(cam, shader);

  if (ctx.wireframeMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  shaderUse(shader);
  glDrawElementsInstanced(GL_TRIANGLES, self->indices, GL_UNSIGNED_INT, 0, self->instanceCount);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  vaoUnbind();
}

void meshInstancedClear(MeshInstanced* self) {
  vaoClear(&self->vao);
  GLBuffer_clear(&self->vbo);
  GLBuffer_clear(&self->vboInstanced);
  GLBuffer_clear(&self->ebo);
}

