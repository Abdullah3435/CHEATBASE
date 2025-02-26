#include "memory.h"

uintptr_t memory::GetProcID(const wchar_t* process) // get the orocess id from name
{
	HANDLE hand;
	hand = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hand, &proc);

	do
	{
		if (!wcscmp(process, proc.szExeFile))
		{
			CloseHandle(hand);
			pID = proc.th32ProcessID;
			Handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pID);
			return proc.th32ProcessID;
		}
	} while (Process32Next(hand, &proc));

	CloseHandle(hand);
	return 0;
}

uintptr_t memory::GetModuleBaseAddress(uintptr_t procID, const wchar_t* module) //fetch base address directly
{
	HANDLE handle;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	MODULEENTRY32 mod;
	mod.dwSize = sizeof(MODULEENTRY32);
	Module32First(handle, &mod);

	do
	{
		if (!wcscmp(module, mod.szModule))
		{
			CloseHandle(handle);
			return (uintptr_t)mod.modBaseAddr;
		}
	} while (Module32Next(handle, &mod));

	CloseHandle(handle);
	return 0;
}

