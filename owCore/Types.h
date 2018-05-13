#pragma once

// General types
typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

// Misc functions
#define OW_UNUSED_VAR(a)     ((void)(a))
#define OW_ZERO_MEM(a)       (memset(a, 0, sizeof(a)));
#define OW_COUNT_ELEMENTS(a) ((sizeof(a)) / (sizeof(a[0])))

// Math types
#include "Math.h"
typedef Vec2f vec2;
typedef const Vec2f& cvec2;
typedef Vec3f vec3;
typedef const Vec3f& cvec3;
typedef Vec4f vec4;
typedef const Vec4f& cvec4;
typedef Matrix4f mat4;
typedef const Matrix4f& cmat4;
typedef Quaternion quat;
typedef const Quaternion& cquat;

// Own types
#include "Color.h"
#include "Primitives.h"
#include "Timer.h"

// STL Types
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

typedef const std::string& cstring;