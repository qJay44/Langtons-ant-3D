#version 460 core

out vec4 FragColor;

in vec3 v_rayDir;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform vec3 u_camPos;
uniform uint u_gridSize;

layout(binding = 0) uniform usampler3D u_voxelGrid;
layout(binding = 1) uniform samplerCube u_skybox;

#define RAY_COUNT 3

// Linear
const vec3 stateColors[10] = vec3[] (
  vec3(1.0000f, 0.0191f, 0.0191f), // State 1: Red
  vec3(0.0191f, 0.6918f, 0.0191f), // State 2: Green
  vec3(0.0191f, 0.1717f, 1.0000f), // State 3: Blue
  vec3(1.0000f, 0.5776f, 0.0000f), // State 4: Yellow
  vec3(0.0000f, 1.0000f, 1.0000f), // State 5: Cyan
  vec3(1.0000f, 0.0000f, 1.0000f), // State 6: Magenta
  vec3(0.4012f, 0.0191f, 1.0000f), // State 7: Purple / Indigo
  vec3(0.3564f, 1.0000f, 0.0000f), // State 8: Bright Lime Green
  vec3(1.0000f, 0.0732f, 0.0000f), // State 9: Intense Orange / Coral
  vec3(1.0000f, 0.0191f, 0.3564f)  // State 10: Hot Pink
);

struct Ray {
  vec3 origin;
  vec3 dir;
};

struct HitInfo {
  bool hit;
  ivec3 coord;
  vec3 normal;
  float dist;
  uint state;
};

HitInfo rayMarchDDA(Ray ray, int maxSteps) {
  HitInfo info;
  info.hit = false;

  vec3 coordf = floor(ray.origin);
  ivec3 coord = ivec3(coordf);
  vec3 deltaDist = abs(vec3(1.f) / ray.dir);
  ivec3 stepDir = ivec3(sign(ray.dir));

  vec3 sideDist;
  if (ray.dir.x < 0.f) sideDist.x = (ray.origin.x - coordf.x) * deltaDist.x;
  else                 sideDist.x = (coordf.x + 1.f - ray.origin.x) * deltaDist.x;

  if (ray.dir.y < 0.f) sideDist.y = (ray.origin.y - coordf.y) * deltaDist.y;
  else                 sideDist.y = (coordf.y + 1.f - ray.origin.y) * deltaDist.y;

  if (ray.dir.z < 0.f) sideDist.z = (ray.origin.z - coordf.z) * deltaDist.z;
  else                 sideDist.z = (coordf.z + 1.f - ray.origin.z) * deltaDist.z;

  vec3 mask = vec3(0.f);

  // Maybe add a uniform here
  for(int i = 0; i < maxSteps; i++){
    if (coord.x < 0 || coord.x >= u_gridSize ||
        coord.y < 0 || coord.y >= u_gridSize ||
        coord.z < 0 || coord.z >= u_gridSize) break;

    uint voxelState = texelFetch(u_voxelGrid, coord, 0).r;

    if (voxelState > 0) {
      info.hit = true;
      info.state = voxelState;
      info.coord = coord;
      info.normal = -vec3(stepDir) * mask;

      if (mask.x > 0.f) info.dist = sideDist.x - deltaDist.x;
      else if (mask.y > 0.f) info.dist = sideDist.y - deltaDist.y;
      else info.dist = sideDist.z - deltaDist.z;

      return info;
    }

    if (sideDist.x < sideDist.y) {
      if (sideDist.x < sideDist.z) {
        sideDist.x += deltaDist.x;
        coord.x += stepDir.x;
        mask = vec3(1.f, 0.f, 0.f);
      } else {
        sideDist.z += deltaDist.z;
        coord.z += stepDir.z;
        mask = vec3(0.f, 0.f, 1.f);
      }
    } else {
      if (sideDist.y < sideDist.z) {
        sideDist.y += deltaDist.y;
        coord.y += stepDir.y;
        mask = vec3(0.f, 1.f, 0.f);
      } else {
        sideDist.z += deltaDist.z;
        coord.z += stepDir.z;
        mask = vec3(0.f, 0.f, 1.f);
      }
    }
  }

  return info;
}

// 0.f - shadow, 1.f - light source
float calcShadow(vec3 origin, float maxDist) {
  vec3 coordf = floor(origin);
  ivec3 coord = ivec3(coordf);
  vec3 deltaDist = abs(vec3(1.f) / u_lightDir);
  ivec3 stepDir = ivec3(sign(u_lightDir));

  vec3 sideDist;
  if (u_lightDir.x < 0.f) sideDist.x = (origin.x - coordf.x) * deltaDist.x;
  else                    sideDist.x = (coordf.x + 1.f - origin.x) * deltaDist.x;

  if (u_lightDir.y < 0.f) sideDist.y = (origin.y - coordf.y) * deltaDist.y;
  else                    sideDist.y = (coordf.y + 1.f - origin.y) * deltaDist.y;

  if (u_lightDir.z < 0.f) sideDist.z = (origin.z - coordf.z) * deltaDist.z;
  else                    sideDist.z = (coordf.z + 1.f - origin.z) * deltaDist.z;

  float dist = 0.f;

  // Maybe add a uniform here
  for(int i = 0; i < 50; i++){
    if (dist > maxDist)
      return 1.f;

    if (coord.x < 0 || coord.x >= u_gridSize ||
        coord.y < 0 || coord.y >= u_gridSize ||
        coord.z < 0 || coord.z >= u_gridSize)
      return 1.f;

    if (texelFetch(u_voxelGrid, coord, 0).r > 0)
      return 0.f;

    if (sideDist.x < sideDist.y) {
      if (sideDist.x < sideDist.z) {
        sideDist.x += deltaDist.x;
        coord.x += stepDir.x;
      } else {
        sideDist.z += deltaDist.z;
        coord.z += stepDir.z;
      }
    } else {
      if (sideDist.y < sideDist.z) {
        sideDist.y += deltaDist.y;
        coord.y += stepDir.y;
      } else {
        sideDist.z += deltaDist.z;
        coord.z += stepDir.z;
      }
    }
  }

  return 1.f;
}

float calcVoxelAO(ivec3 pos, ivec3 normal) {
  vec3 tangent = normal.y != 0 || normal.z != 0 ? vec3(1.f, 0.f, 0.f) : vec3(0.f, 1.f, 0.f);
  vec3 bitangent = cross(vec3(normal), tangent);
  ivec3 t = ivec3(tangent);
  ivec3 b = ivec3(bitangent);

  uint n1 = texelFetch(u_voxelGrid, pos + t, 0).r;
  uint n2 = texelFetch(u_voxelGrid, pos - t, 0).r;
  uint n3 = texelFetch(u_voxelGrid, pos + b, 0).r;
  uint n4 = texelFetch(u_voxelGrid, pos - b, 0).r;

  float occlusion = (
    float(n1 > 0) +
    float(n2 > 0) +
    float(n3 > 0) +
    float(n4 > 0)
  ) * 0.2f;

  return 1.f - occlusion;
}

float calcFresnel(vec3 viewDir, vec3 normal, float f0) {
  float cosTheta = clamp(dot(-viewDir, normal), 0.f, 1.f);
  return f0 + (1.f - f0) * pow(1.f - cosTheta, 5.f);
}

float calcVoxelEdge(vec3 hitPos) {
  vec3 d = abs(fract(hitPos) - 0.5f);
  float w = 0.48f; // Width
  float softness = 0.02f;
  vec3 smoothEdge = smoothstep(vec3(w), vec3(w + softness), d);

  return max(smoothEdge.x * smoothEdge.y,
         max(smoothEdge.y * smoothEdge.z,
             smoothEdge.z * smoothEdge.x));
}

vec3 calcBeveledNormal(vec3 flatNormal, vec3 hitPos) {
  vec3 d = fract(hitPos) - 0.5f;
  vec3 bevel = smoothstep(0.4f, 0.5f, abs(d));
  bevel *= (1.f - abs(flatNormal)); // Mask out the bevel
  vec3 bentNormal = flatNormal + bevel * sign(d);

  return normalize(bentNormal);
}

vec3 calcSpecular(vec3 normal, vec3 viewDir) {
  vec3 halfwayDir = normalize(u_lightDir + viewDir);
  float specularStrength = 10.5f; // Intensity
  float shininess = 64.f;         // Higher - smaller, sharper, shinier highlights

  float spec = pow(max(dot(normal, halfwayDir), 0.f), shininess);

  return specularStrength * spec * u_lightColor;
}

void main() {
  Ray ray;
  ray.origin = u_camPos;
  ray.dir = normalize(v_rayDir);

  vec3 finalColor = vec3(0.f);
  float intensity = 1.f;
  int maxRaySteps = 128;

  for(int i = 0; i < RAY_COUNT; i++){
    HitInfo info = rayMarchDDA(ray, maxRaySteps);
    float invStep = 1.f;

    if (!info.hit) {
      vec3 skyboxColor = texture(u_skybox, ray.dir).rgb;
      finalColor += skyboxColor * intensity;
      break;
    }

    vec3 hitPos = ray.origin + info.dist * ray.dir;
    vec3 smoothNormal = calcBeveledNormal(info.normal, hitPos); // Use only for lighting
    vec3 albedo = stateColors[info.state - 1];

    vec3 shadowOrigin = hitPos + 0.001f * info.normal;
    ivec3 inormal = ivec3(info.normal);

    float edge = calcVoxelEdge(hitPos);
    albedo = mix(albedo, vec3(1.f), edge);
    // albedo = mix(vec3(0.0063f), albedo, edge); // Also cool

    float ao = calcVoxelAO(info.coord + inormal, inormal);
    float fresnel = calcFresnel(ray.dir, smoothNormal, 0.125f); // More than 1.f produces interesting results
    float diffuse = max(0.f, dot(info.normal, u_lightDir));
    float shadow = calcShadow(shadowOrigin, 50.f);

    vec3 specColor = calcSpecular(smoothNormal, -ray.dir);

    vec3 lighting = albedo * 0.1f * ao;
    lighting += albedo * shadow * (1.f - fresnel) * (diffuse + specColor); // Metallic specular

    finalColor += lighting * intensity;

    ray.origin = shadowOrigin;
    ray.dir = reflect(ray.dir, smoothNormal);

    intensity *= fresnel;
    maxRaySteps >>= 1; // Less quality for reflections
  }

  finalColor = pow(finalColor, vec3(1.f / 2.2f));

  FragColor = vec4(finalColor, 1.f);
}

