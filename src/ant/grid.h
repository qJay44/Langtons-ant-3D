#ifndef GRID_H
#define GRID_H

#include "../engine/mesh/mesh.h"
#include "../engine/mesh/meshInstanced.h"

void gridInitMesh(const MeshData* data);
void gridInitMeshFromOBJ(const char* filepath, u32 attribFlags);

int gridGetVoxel(ivec3s coord);
void gridSetVoxel(ivec3s coord, int state);
void gridUpdateMesh();
void gridClearStates();
void gridClearMesh();

extern MeshInstanced gridMesh;

#endif

