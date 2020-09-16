#pragma once
#include <windows.h>

//Player class
class Player
{
public:
	int IsImposter() { return *reinterpret_cast<int*>((reinterpret_cast<uintptr_t>(this) + 0x28)); }
	int GetID() { return *reinterpret_cast<int*>((reinterpret_cast<uintptr_t>(this) + 0x8)); }
	DWORD GetNamePointer() { return *reinterpret_cast<DWORD*>((reinterpret_cast<uintptr_t>(this) + 0xC)); }
};

uintptr_t Mod = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));

typedef Player* (__cdecl* GetPlayer)(void* Instance, int name_id); //int __cdecl GameData__GetPlayerById(int a1, char a2)
GetPlayer GetPlayerById = (GetPlayer)((Mod + 0x224400));

typedef void(__cdecl* OnGameEnd)(int a1, int32_t reason, char a3); //int __cdecl AmongUsClient__OnGameEnd(int a1, int32_t reason, char a3)
OnGameEnd GameEnd = (OnGameEnd)((Mod + 0x29FDB0));

typedef void(__cdecl* ExitGame)(int a1); //void __cdecl ExitGameButton__Start(int a1)
ExitGame LeaveGame = (ExitGame)((Mod + 0x21F1A0));

typedef  wchar_t(__cdecl* GetCount)(void* Instance); //void __cdecl GameData__get_PlayerCount(int a1)
GetCount GetPlayerCount = (GetCount)((Mod + 0x225730));
