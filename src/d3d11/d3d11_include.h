#pragma once

#if defined(__WINE__)
  #define DLLEXPORT __attribute__((visibility("default")))
#elif defined(_MSC_VER)
  #define DLLEXPORT
#else
  #define DLLEXPORT __declspec(dllexport)
#endif

#include "../util/com/com_guid.h"
#include "../util/com/com_object.h"
#include "../util/com/com_pointer.h"

#include "../util/log/log.h"
#include "../util/log/log_debug.h"

#include "../util/util_flags.h"
#include "../util/util_likely.h"
#include "../util/util_math.h"
#include "../util/util_monitor.h"
#include "../util/util_string.h"

#include <dxgi1_6.h>
#include <d3d11_4.h>

