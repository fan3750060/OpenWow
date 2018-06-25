#pragma once

#define VERSION_Vanila  0
#define VERSION_BC		1
#define VERSION_WotLK	2

#define VERSION VERSION_WotLK

// General types
typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

// Math types
class Vec2f;
class Vec3f;
class Vec4f;
class Matrix4f;
class Quaternion;

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

class Color;

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN 1
#endif
#ifndef NOMINMAX
#   define NOMINMAX
#endif
#include <windows.h>

// STL Types
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
using namespace std;

typedef const string& cstring;

// Consts
enum TextAlignW
{
	TEXT_ALIGNW_LEFT = 0,
	TEXT_ALIGNW_CENTER,
	TEXT_ALIGNW_RIGHT
};

enum TextAlignH
{
	TEXT_ALIGNH_TOP = 0,
	TEXT_ALIGNH_CENTER,
	TEXT_ALIGNH_BOTTOM
};

enum InputMode
{
	MODE_CHARACTERS = 0,
	MODE_NUMBERS,
	MODE_ANY
};

// Usefull macros
#define __PACK_BEGIN  "../shared/pack_begin.h"
#define __PACK_END  "../shared/pack_end.h"