#include "meshInstanced.h"

#include "GLBuffer.h"
#include "Context.h"

void meshInstancedDraw(MeshInstanced* self, const Camera* cam, Shader* shader) {
  if (self->instanceCount == 0)
    return;

  vaoBind(&self->vao);

  cameraSetUniforms(cam, shader);

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

