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
	char pad_0000[104]; //0x0000
	wchar_t* name; //0x0068
	char pad_0070[2952]; //0x0070
	int32_t speedbuff; //0x0BF8
	int32_t attackbuff; //0x0BFC
	int32_t castbuff; //0x0C00
	char pad_0C04[1104]; //0x0C04
}; //Size: 0x1054

extern void CollectOffsets();