#pragma once

#if ION_VULKAN
#include "Vulkan/Buffer.h"
#include "Vulkan/Image.h"
#else
#include "DX11/Buffer.h"
#include "DX11/Image.h"
#endif