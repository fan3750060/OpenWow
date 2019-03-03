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

// GLM
#pragma warning( push )
#pragma warning( disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#pragma warning( pop ) 

typedef glm::vec2 vec2;
typedef const vec2& cvec2;
typedef glm::vec3 vec3;
typedef const vec3& cvec3;
typedef glm::vec4 vec4;
typedef const vec4& cvec4;
typedef glm::mat4x4 mat4;
typedef const mat4& cmat4;
typedef glm::quat quat;
typedef const quat& cquat;

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
#include <mutex>

typedef const std::string& cstring;

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