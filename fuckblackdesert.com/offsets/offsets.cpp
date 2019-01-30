#include "offsets.h"


offsets_t offys;

void CollectOffsets() {
	offys.f_cam_max_dist = (Memory::ReadPtr(Memory::FindPatternIDA("BlackDesert64.exe", "F3 0F 10 0D ? ? ? ? 48 8D 8E ? ? ? ? F3 0F 10 01 48 8D 85 ? ? ? ? F3 0F 10 15 ? ? ? ? F3 41 0F 59 C9 0F 2F C1 F3 0F 11 8D ? ? ? ? 48 0F 46 C1"), 4));
	offys.dw_localplayer = (Memory::ReadPtr(Memory::FindPatternIDA("BlackDesert64.exe","48 8B 05 ? ? ? ? 48 85 C0 74 29 8B 40 58"), 3));
	offys.dw_clientspeedac = Memory::FindPatternIDA("BlackDesert64.exe", "80 FA 03 0F 83 ? ? ? ?");
	offys.dw_clientattackac = Memory::FindPatternIDA("BlackDesert64.exe", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 20 33 ED 48 8D B9 ? ? ? ?");
}

