#include "mesh.h"

#include <stdio.h>

#include "GLBuffer.h"
#include "fast_obj.h"
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

void meshLoadObjPN(const char* filepath, MeshData* outData) {
  fastObjMesh* obj = fast_obj_read(filepath);
  if (!obj) {
    fprintf(stderr, "❌Failed to open OBJ: [%s]\n", filepath);
    exit(EXIT_FAILURE);
  }

  u32 totalTriangles = 0;
  for (u32 i = 0; i < obj->face_count; i++)
    totalTriangles += obj->face_vertices[i] - 2;

  u32 indicesCount = totalTriangles * 3;

  outData->vertSize = indicesCount * 6 * sizeof(float);
  outData->indSize  = indicesCount * sizeof(GLuint);

  outData->vertices = malloc(outData->vertSize);
  outData->indices  = malloc(outData->indSize);

  if (!outData->vertices || !outData->indices) {
    fprintf(stderr, "❌ Memory allocation failed while loading OBJ: [%s]\n", filepath);
    exit(EXIT_FAILURE);
  }

  u32 vertexTracker = 0;
  u32 indexTracker = 0;
  u32 objIndexOffset = 0; // Exact position in the raw OBJ index pool

  for (u32 i = 0; i < obj->face_count; i++) {
    u32 faceVerts = obj->face_vertices[i];

    // Fan triangulate polygons (safely turns quads/n-gons into multiple triangles)
    for (u32 j = 0; j < faceVerts - 2; j++) {
      u32 cornerIndices[3];
      cornerIndices[0] = objIndexOffset;
      cornerIndices[1] = objIndexOffset + j + 1;
      cornerIndices[2] = objIndexOffset + j + 2;

      for (u32 k = 0; k < 3; k++) {
        fastObjIndex idx = obj->indices[cornerIndices[k]];

        outData->vertices[vertexTracker + 0] = obj->positions[idx.p * 3 + 0];
        outData->vertices[vertexTracker + 1] = obj->positions[idx.p * 3 + 1];
        outData->vertices[vertexTracker + 2] = obj->positions[idx.p * 3 + 2];

        if (idx.n) {
          outData->vertices[vertexTracker + 3] = obj->normals[idx.n * 3 + 0];
          outData->vertices[vertexTracker + 4] = obj->normals[idx.n * 3 + 1];
          outData->vertices[vertexTracker + 5] = obj->normals[idx.n * 3 + 2];
        } else {
          outData->vertices[vertexTracker + 3] = 0.f;
          outData->vertices[vertexTracker + 4] = 0.f;
          outData->vertices[vertexTracker + 5] = 0.f;
        }

        outData->indices[indexTracker] = indexTracker;

        indexTracker++;
        vertexTracker += 6;
      }
    }
    objIndexOffset += faceVerts;
  }

  fast_obj_destroy(obj);
}

Mesh meshCreatePN(const MeshData* data) {
  Mesh mesh;
  mesh.vertices = data->vertSize / sizeof(data->vertices[0]);
  mesh.indices = data->indSize / sizeof(data->indices[0]);
  mesh.mats.trans = glms_mat4_identity();
  mesh.mats.rot   = glms_mat4_identity();
  mesh.mats.scale = glms_mat4_identity();

  bool useEBO = mesh.indices > 0;

  vaoGen(&mesh.vao, 1);
  GLBuffer_gen(&mesh.vbo, GL_ARRAY_BUFFER, 1);
  if (useEBO) GLBuffer_gen(&mesh.ebo, GL_ELEMENT_ARRAY_BUFFER, 1);

  vaoBind(&mesh.vao);

  GLBuffer_allocate(&mesh.vbo, data->vertices, data->vertSize, GL_STATIC_DRAW);
  if (useEBO) GLBuffer_allocate(&mesh.ebo, data->indices, data->indSize, GL_STATIC_DRAW);

  vaoBind(&mesh.vao);
  GLBuffer_bind(&mesh.vbo);
  if (useEBO) GLBuffer_bind(&mesh.ebo);

  size_t typeSize = sizeof(float);
  size_t stride = typeSize * 3 * 2;
  vaoLinkAttrib(0, 3, GL_FLOAT, stride, (void*)(0 * typeSize));
  vaoLinkAttrib(1, 3, GL_FLOAT, stride, (void*)(3 * typeSize));

  vaoUnbind();
  GLBuffer_unbind(&mesh.vbo);
  // No need to unbound EBO (VAO unbinding is enough)

  return mesh;
}

void meshDraw(Mesh* self, const Camera* cam, Shader* shader) {
  vaoBind(&self->vao);

  mat4s* mats[3] = {&self->mats.trans, &self->mats.rot, &self->mats.scale};
  mat4s model = glms_mat4_mulN(mats, 3);

  setCameraUniforms(cam, shader);
  shaderSetUniformMat4f(shader, "u_model", model.raw);

  shaderUse(shader);
  glDrawElements(GL_TRIANGLES, self->indices, GL_UNSIGNED_INT, 0);

  vaoUnbind();
}

void meshDrawScreen(const Camera* cam, Shader* shader) {
  const VAO* vao = vaoEmpty();
  vaoBind(vao);

  setCameraUniforms(cam, shader);
  shaderUse(shader);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  vaoUnbind();
}

void meshClear(Mesh* self) {
  vaoClear(&self->vao);
  GLBuffer_clear(&self->vbo);
  GLBuffer_clear(&self->ebo);
}

