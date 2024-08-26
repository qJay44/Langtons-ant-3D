#include "mesh.h"

#include <string.h>

#include "cglm/affine-pre.h"
#include "cglm/struct/mat4.h"

#include "shader.h"
#include "vao.h"
#include "vbo.h"

Mesh meshCreate(float* vertices, size_t vertSize, GLuint* indices, size_t indSize) {
  Mesh mesh;
  mesh.vertices = vertices;
  mesh.vertSize = vertSize;
  mesh.indices = indices;
  mesh.indSize = indSize;
  mesh.mat = (mat4s)GLMS_MAT4_IDENTITY_INIT;

  mesh.vao = vaoCreate(1);
  mesh.vbo = vboCreate(1, mesh.vertices, mesh.vertSize);
  mesh.ebo = eboCreate(1, mesh.indices, mesh.indSize);

  // Bind
  vaoBind(&mesh.vao);
  vboBind(&mesh.vbo);
  eboBind(&mesh.ebo);

  // ===== Link attributes ===== //

  size_t typeSize = sizeof(GLfloat);
  size_t stride = MESH_VERTEX_ATTRIBUTES * typeSize;

  vaoLinkAttrib(0, 3, GL_FLOAT, stride, (void*)(0 * typeSize)); // 1. Position
  vaoLinkAttrib(1, 3, GL_FLOAT, stride, (void*)(3 * typeSize)); // 2. Color
  vaoLinkAttrib(2, 2, GL_FLOAT, stride, (void*)(6 * typeSize)); // 3. Texture
  vaoLinkAttrib(3, 3, GL_FLOAT, stride, (void*)(8 * typeSize)); // 4. Normal

  // =========================== //

  // Unbind
  vaoUnbind();
  vboUnbind();
  eboUnbind();

  return mesh;
}

Mesh meshCreateCube(float size, vec3s pos, vec3s color, float scale) {
  //        5--------6
  //       /|       /|
  //      4--------7 |
  //      | |      | |
  //      | 1------|-2
  //      |/       |/
  //      0--------3

  float verticesStack[88] = {
    -size, -size,  size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
    -size, -size, -size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
     size, -size, -size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
     size, -size,  size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
    -size,  size,  size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
    -size,  size, -size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
     size,  size, -size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
     size,  size,  size,  color.x, color.y, color.z,   0.f, 0.f,   1.f, 1.f, 0.f,
  };
  size_t vertSize = sizeof(verticesStack);
  float* vertices = malloc(vertSize);

  GLuint indicesStack[36] = {
	  0, 1, 2,
	  0, 2, 3,
	  0, 4, 7,
	  0, 7, 3,
	  3, 7, 6,
	  3, 6, 2,
	  2, 6, 5,
	  2, 5, 1,
	  1, 5, 4,
	  1, 4, 0,
	  4, 5, 6,
	  4, 6, 7
  };
  size_t indSize = sizeof(indicesStack);
  GLuint* indices = malloc(indSize);

  memcpy((void*)vertices, (void*)verticesStack, vertSize);
  memcpy((void*)indices, (void*)indicesStack, indSize);

  Mesh mesh = meshCreate(vertices, vertSize, indices, indSize);
  meshScale(&mesh, scale);
  meshTranslate(&mesh, pos);

  return mesh;
}

void meshScale(Mesh* self, float s) {
  glm_mat4_scale(self->mat.raw, s);
}

void meshTranslate(Mesh* self, vec3s v) {
  glm_translate(self->mat.raw, v.raw);
}

void meshTranslateX(Mesh* self, float x) {
  glm_translate_x(self->mat.raw, x);
}

void meshTranslateY(Mesh* self, float y) {
  glm_translate_y(self->mat.raw, y);
}

void meshTranslateZ(Mesh* self, float z) {
  glm_translate_z(self->mat.raw, z);
}

Mesh meshTranslateCopy(const Mesh* mesh, vec3s v) {
  Mesh m = *mesh;
  glm_translate(m.mat.raw, v.raw);

  return m;
}

void meshDraw(Mesh* self, const Shader* shader) {
  vaoBind(&self->vao);

  shaderUniformVec3(shader, "camPos", _gCamera.position.raw);
  shaderUniformMat4(shader, "cam", _gCamera.mat.raw);

  shaderUniformMat4(shader, "model", self->mat.raw);
  glDrawElements(GL_TRIANGLES, self->indSize / sizeof(self->indices[0]), GL_UNSIGNED_INT, 0);

  vaoUnbind();
}

void meshDelete(Mesh* self) {
  vaoDelete(&self->vao);
  vboDelete(&self->vbo);
  eboDelete(&self->ebo);

  free(self->vertices);
  free(self->indices);

}

