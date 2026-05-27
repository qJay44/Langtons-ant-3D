#include "mesh.h"

#include <stdio.h>

#include "Context.h"
#include "GLBuffer.h"
#include "cglm/struct/mat4.h"
#include "vertex.h"

static void linkAttributes(const MeshData* data, size_t elementSize) {
  size_t offset = 0;
  for (size_t i = 0; i < data->layout.count; i++) {
    VertexAttribute attr = data->layout.attribs[i];
    vaoLinkAttrib(i, attr.size, attr.type, data->layout.stride, (void*)(offset));
    offset += attr.size * elementSize;
  }
}

static void setGlobalUniforms(Shader* shader) {
  vec2s ws = getWinSizef();

  shaderSetUniform2f(shader, "u_resolution", ws.raw);
}

MeshElements meshCreateElements(const MeshData* data) {
  MeshElements mesh;
  mesh.count = data->indicesSize / sizeof(data->indices[0]);
  mesh.mats.trans = glms_mat4_identity();
  mesh.mats.rot   = glms_mat4_identity();
  mesh.mats.scale = glms_mat4_identity();

  vaoGen(&mesh.vao, 1);
  GLBuffer_gen(&mesh.vbo, GL_ARRAY_BUFFER, 1);
  GLBuffer_gen(&mesh.ebo, GL_ELEMENT_ARRAY_BUFFER, 1);

  vaoBind(&mesh.vao);

  GLBuffer_allocate(&mesh.vbo, data->vertices, data->verticesSize, GL_STATIC_DRAW);
  GLBuffer_allocate(&mesh.ebo, data->indices, data->indicesSize, GL_STATIC_DRAW);

  vaoBind(&mesh.vao);
  GLBuffer_bind(&mesh.vbo);
  GLBuffer_bind(&mesh.ebo);

  linkAttributes(data, sizeof(float));

  vaoUnbind();
  GLBuffer_unbind(&mesh.vbo);
  // No need to unbound EBO (VAO unbinding is enough)

  return mesh;
}

MeshArrays meshCreateArrays(const MeshData* data, GLenum usage) {
  MeshArrays mesh;
  mesh.count = data->verticesSize / sizeof(data->vertices[0]);
  mesh.mats.trans = glms_mat4_identity();
  mesh.mats.rot   = glms_mat4_identity();
  mesh.mats.scale = glms_mat4_identity();

  vaoGen(&mesh.vao, 1);
  GLBuffer_gen(&mesh.vbo, GL_ARRAY_BUFFER, 1);

  vaoBind(&mesh.vao);

  GLBuffer_allocate(&mesh.vbo, data->vertices, data->verticesSize, usage);

  vaoBind(&mesh.vao);
  GLBuffer_bind(&mesh.vbo);

  linkAttributes(data, sizeof(float));

  vaoUnbind();
  GLBuffer_unbind(&mesh.vbo);

  return mesh;
}

void meshUpdateBufferVBO(MeshArrays* self, const MeshData* data, GLintptr offset) {
  GLBuffer_update(&self->vbo, data->vertices, data->verticesSize, offset);
  self->count = data->verticesSize / data->layout.stride;
}

void meshDrawElements(MeshElements* self, const Camera* cam, Shader* shader) {
  vaoBind(&self->vao);

  mat4s* mats[3] = {&self->mats.trans, &self->mats.rot, &self->mats.scale};
  mat4s model = glms_mat4_mulN(mats, 3);

  cameraSetUniforms(cam, shader);
  setGlobalUniforms(shader);
  shaderSetUniformMat4f(shader, "u_model", model.raw);

  shaderUse(shader);
  glDrawElements(GL_TRIANGLES, self->count, GL_UNSIGNED_INT, 0);

  vaoUnbind();
}

void meshDrawArrays(MeshArrays* self, const Camera* cam, Shader* shader) {
  vaoBind(&self->vao);

  mat4s* mats[3] = {&self->mats.trans, &self->mats.rot, &self->mats.scale};
  mat4s model = glms_mat4_mulN(mats, 3);

  cameraSetUniforms(cam, shader);
  setGlobalUniforms(shader);
  shaderSetUniformMat4f(shader, "u_model", model.raw);

  shaderUse(shader);
  glDrawArrays(GL_TRIANGLES, 0, self->count);

  vaoUnbind();
}

void meshDrawScreen(const Camera* cam, Shader* shader) {
  const VAO* vao = vaoEmpty();
  vaoBind(vao);

  cameraSetUniforms(cam, shader);
  setGlobalUniforms(shader);
  shaderUse(shader);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  vaoUnbind();
}

void meshClear(MeshElements* self) {
  vaoClear(&self->vao);
  GLBuffer_clear(&self->vbo);
  GLBuffer_clear(&self->ebo);
}

