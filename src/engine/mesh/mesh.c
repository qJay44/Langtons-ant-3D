#include "mesh.h"

#include <stdio.h>

#include "GLBuffer.h"
#include "fast_obj.h"
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

void meshLoadObj(const char* filepath, MeshData* ioData, u32 attribFlags) {
  fastObjMesh* obj = fast_obj_read(filepath);
  if (!obj) {
    fprintf(stderr, "❌Failed to open OBJ: [%s]\n", filepath);
    exit(EXIT_FAILURE);
  }

  u32 totalTriangles = 0;
  for (u32 i = 0; i < obj->face_count; i++)
    totalTriangles += obj->face_vertices[i] - 2;

  u32 indicesCount = totalTriangles * 3;

  ioData->verticesSize = indicesCount * ioData->layout.stride;
  ioData->indicesSize  = indicesCount * sizeof(ioData->indices[0]);

  ioData->vertices = malloc(ioData->verticesSize);
  ioData->indices  = malloc(ioData->indicesSize);

  if (!ioData->vertices || !ioData->indices) {
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
        u32 vTrackerOffset = 0;

        if ((attribFlags & MESH_LOAD_OBJ_ATTRIB_POSITION) && idx.p) {
          ioData->vertices[vertexTracker + 0] = obj->positions[idx.p * 3 + 0];
          ioData->vertices[vertexTracker + 1] = obj->positions[idx.p * 3 + 1];
          ioData->vertices[vertexTracker + 2] = obj->positions[idx.p * 3 + 2];
          vTrackerOffset += 3;
        }

        if ((attribFlags & MESH_LOAD_OBJ_ATTRIB_TEXTURE) && idx.t) {
          ioData->vertices[vertexTracker + vTrackerOffset + 0] = obj->texcoords[idx.t * 2 + 0];
          ioData->vertices[vertexTracker + vTrackerOffset + 1] = obj->texcoords[idx.t * 2 + 1];
          vTrackerOffset += 2;
        }

        if ((attribFlags & MESH_LOAD_OBJ_ATTRIB_NORMAL) && idx.n) {
          ioData->vertices[vertexTracker + vTrackerOffset + 0] = obj->normals[idx.n * 3 + 0];
          ioData->vertices[vertexTracker + vTrackerOffset + 1] = obj->normals[idx.n * 3 + 1];
          ioData->vertices[vertexTracker + vTrackerOffset + 2] = obj->normals[idx.n * 3 + 2];
          vTrackerOffset += 3;
        }

        ioData->indices[indexTracker] = indexTracker;

        indexTracker++;
        vertexTracker += vTrackerOffset;
      }
    }
    objIndexOffset += faceVerts;
  }

  fast_obj_destroy(obj);
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
  shaderSetUniformMat4f(shader, "u_model", model.raw);

  shaderUse(shader);
  glDrawArrays(GL_TRIANGLES, 0, self->count);

  vaoUnbind();
}

void meshDrawScreen(const Camera* cam, Shader* shader) {
  const VAO* vao = vaoEmpty();
  vaoBind(vao);

  cameraSetUniforms(cam, shader);
  shaderUse(shader);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  vaoUnbind();
}

void meshClear(MeshElements* self) {
  vaoClear(&self->vao);
  GLBuffer_clear(&self->vbo);
  GLBuffer_clear(&self->ebo);
}

