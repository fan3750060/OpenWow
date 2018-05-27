#pragma once

#include <algorithm>
#include <functional>
#include <fstream>
#include <cmath>
#include <memory>
using namespace std;

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#include <windows.h>

// Macros
#include "Debug.h"
#include "Macros.h"

// Primitives
#include "Math.h"
#include "BoundingBox.h"
#include "Frustrum.h"
#include "Plane.h"

// Additional types
#include "Color.h"
#include "Function.h"
#include "FunctionWA.h"
#include "FunctionSA.h"
#include "Timer.h"

// Utils
#include "Random.h"
#include "Utils.h"

#include "Module.h"
#include "Modules.h"

#include "Log.h"