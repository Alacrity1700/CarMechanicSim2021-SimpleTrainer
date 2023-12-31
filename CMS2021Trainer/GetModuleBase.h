#include "baseIncludes.h"


uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) 
{
	uintptr_t modBaseAdd = 0;

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					modBaseAdd = (uintptr_t)modEntry.modBaseAddr;
					break;

				}


			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAdd;
}
