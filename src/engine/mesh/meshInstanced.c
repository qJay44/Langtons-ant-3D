#include "meshInstanced.h"

#include <stdio.h>

#include "GLBuffer.h"
#include "cglm/struct/mat4.h"

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

MeshInstanced meshInstancedCreatePN(float* vertices, size_t vertSize, GLuint* indices, size_t indSize) {
  MeshInstanced mesh;
  mesh.indices = indSize / sizeof(indices[0]);

  vaoGen(&mesh.vao, 1);
  GLBuffer_gen(&mesh.vbo, GL_ARRAY_BUFFER, 1);
  GLBuffer_gen(&mesh.vboInstanced, GL_ARRAY_BUFFER, 1);
  GLBuffer_gen(&mesh.ebo, GL_ELEMENT_ARRAY_BUFFER, 1);

  vaoBind(&mesh.vao);

  GLBuffer_allocate(&mesh.vbo, vertices, vertSize, GL_STATIC_DRAW);
  GLBuffer_allocate(&mesh.ebo, indices, indSize, GL_STATIC_DRAW);

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

MeshInstanced meshInstancedCreateCubePN() {
  float vertices[144] = {
    // Position (x, y, z)       // Normal (x, y, z)
    // FRONT FACE (Z+)
    -1.0f, -1.0f,  1.0f,         0.0f,  0.0f,  1.0f, // 0
     1.0f, -1.0f,  1.0f,         0.0f,  0.0f,  1.0f, // 1
     1.0f,  1.0f,  1.0f,         0.0f,  0.0f,  1.0f, // 2
    -1.0f,  1.0f,  1.0f,         0.0f,  0.0f,  1.0f, // 3

    // BACK FACE (Z-)
     1.0f, -1.0f, -1.0f,         0.0f,  0.0f, -1.0f, // 4
    -1.0f, -1.0f, -1.0f,         0.0f,  0.0f, -1.0f, // 5
    -1.0f,  1.0f, -1.0f,         0.0f,  0.0f, -1.0f, // 6
     1.0f,  1.0f, -1.0f,         0.0f,  0.0f, -1.0f, // 7

    // LEFT FACE (X-)
    -1.0f, -1.0f, -1.0f,        -1.0f,  0.0f,  0.0f, // 8
    -1.0f, -1.0f,  1.0f,        -1.0f,  0.0f,  0.0f, // 9
    -1.0f,  1.0f,  1.0f,        -1.0f,  0.0f,  0.0f, // 10
    -1.0f,  1.0f, -1.0f,        -1.0f,  0.0f,  0.0f, // 11

    // RIGHT FACE (X+)
     1.0f, -1.0f,  1.0f,         1.0f,  0.0f,  0.0f, // 12
     1.0f, -1.0f, -1.0f,         1.0f,  0.0f,  0.0f, // 13
     1.0f,  1.0f, -1.0f,         1.0f,  0.0f,  0.0f, // 14
     1.0f,  1.0f,  1.0f,         1.0f,  0.0f,  0.0f, // 15

    // TOP FACE (Y+)
    -1.0f,  1.0f,  1.0f,         0.0f,  1.0f,  0.0f, // 16
     1.0f,  1.0f,  1.0f,         0.0f,  1.0f,  0.0f, // 17
     1.0f,  1.0f, -1.0f,         0.0f,  1.0f,  0.0f, // 18
    -1.0f,  1.0f, -1.0f,         0.0f,  1.0f,  0.0f, // 19

    // BOTTOM FACE (Y-)
    -1.0f, -1.0f, -1.0f,         0.0f, -1.0f,  0.0f, // 20
     1.0f, -1.0f, -1.0f,         0.0f, -1.0f,  0.0f, // 21
     1.0f, -1.0f,  1.0f,         0.0f, -1.0f,  0.0f, // 22
    -1.0f, -1.0f,  1.0f,         0.0f, -1.0f,  0.0f,  // 23
  };
  size_t vertSize = sizeof(vertices);

  GLuint indices[36] = {
    0,  1,  2,    2,  3,  0,  // Front
    4,  5,  6,    6,  7,  4,  // Back
    8,  9,  10,   10, 11, 8,  // Left
    12, 13, 14,   14, 15, 12, // Right
    16, 17, 18,   18, 19, 16, // Top
    20, 21, 22,   22, 23, 20  // Bottom
  };
  size_t indSize = sizeof(indices);

  return meshInstancedCreatePN(vertices, vertSize, indices, indSize);
}

void meshInstancedDraw(MeshInstanced* self, const Camera* cam, Shader* shader) {
  if (self->instanceCount == 0)
    return;

  vaoBind(&self->vao);

  setCameraUniforms(cam, shader);

  shaderUse(shader);
  glDrawElementsInstanced(GL_TRIANGLES, self->indices, GL_UNSIGNED_INT, 0, self->instanceCount);

  vaoUnbind();
}

void meshInstancedClear(MeshInstanced* self) {
  vaoClear(&self->vao);
  GLBuffer_clear(&self->vbo);
  GLBuffer_clear(&self->vboInstanced);
  GLBuffer_clear(&self->ebo);
}

