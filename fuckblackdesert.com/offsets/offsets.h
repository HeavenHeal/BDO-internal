#pragma once
#include "../main.h"

struct offsets_t {
	uint64_t f_cam_max_dist;
	uint64_t dw_localplayer;
	uint64_t dw_clientspeedac;
	uint64_t dw_clientattackac;
}; extern offsets_t offys;

class CLocalPlayer
{
public:
	char pad_0000[3096]; //0x0000
	int32_t movespeed; //0x0C18
	int32_t attackspeed; //0x0C1C
	int32_t castspeed; //0x0C20
	char pad_0C24[5148]; //0x0C24
}; //Size: 0x2040

extern void CollectOffsets();