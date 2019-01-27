#pragma once
// ================================================
//  system imports
// ================================================

#include <Windows.h>
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <random>
#include <Psapi.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <assert.h>

// ================================================
//  d3d9 header & lib imports
// ================================================


// ================================================
//  custom definitions and imports
// ================================================



namespace Memory {
	DWORD_PTR FindPatternIDA(const char* szModule, const char* szSignature);
	SIZE_T ReadPtr(SIZE_T Address, UINT Offset);
};


#include "../memory/hook.h"
#include "../singleton.h"