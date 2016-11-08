#include "myLib.h"

// Vector2i Scale2i(Vector2i v, i32 s) { return {v.x * s, v.y * s}; }

// Vector3i Scale3i(Vector3i v, i32 s) { return {v.x * s, v.y * s, v.z * s}; }

// Vector2f Scale2f(Vector2f v, FIXED s) {
//   return {fixedMul(v.x, s), fixedMul(v.y, s), fixedMul(v.z, s)};
// }

// Vector3f Scale3f(Vector3f v, FIXED s) {
//   return {fixedMul(v.x, s), fixedMul(v.y, s), fixedMul(v.z, s)};
// }

Vector3f Vec3fAdd(Vector3f a, Vector3f b) {
  Vector3f ret = {a.x + b.x, a.y + b.y, a.z + b.z};
  return ret;
}