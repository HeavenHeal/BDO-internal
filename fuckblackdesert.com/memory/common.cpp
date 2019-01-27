#include "common.h"

#define in_range(x, a, b) (x >= a && x <= b) 
#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

namespace Memory {
	DWORD_PTR FindPatternIDA(const char* szModule, const char* szSignature)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD_PTR startAddress = (DWORD_PTR)GetModuleHandleA(szModule);
		DWORD_PTR endAddress = (startAddress + (DWORD_PTR)modInfo.SizeOfImage);
		const char* pat = szSignature;
		DWORD_PTR firstMatch = 0;
		for (DWORD_PTR pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == ('\?') || *(BYTE*)pCur == get_byte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == ('\?\?') || *(PBYTE)pat != ('\?')) pat += 3;
				else pat += 2; 
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}
	SIZE_T ReadPtr(SIZE_T Address, UINT Offset)
	{
		if (Address == 0)
			return 0;

		if (sizeof(SIZE_T) == 8)
			return Address + (INT)((*(INT*)(Address + Offset) + Offset) + sizeof(INT));

		return (SIZE_T)*(DWORD*)(Address + Offset);

	}
};