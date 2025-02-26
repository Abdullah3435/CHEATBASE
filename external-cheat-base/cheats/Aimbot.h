#pragma once

#include "../SDK/Client.h"
#include "../SDK/Offsets.h"

#include "../memory/memory.h"
#include "../math/vector.h"

#include <Windows.h>
#include <cmath> // For angle calculation
#include <iostream>

#include <cstdint>
#include <string>  // For std::wstring

namespace Aimbot
{
	inline uintptr_t base;
	inline uintptr_t target_process;


	void MoveCursorBasedOnAngle(vec3 selfEye, vec3 enemytransform);
	float dist(vec3 p1, vec3 p2);
	void frame();// per second frame calls
}