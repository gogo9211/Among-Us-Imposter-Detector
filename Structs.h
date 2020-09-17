#pragma once
#include <windows.h>
#include <cstdint>

class player
{
public:
	int is_imposter() { return *reinterpret_cast<int*>((reinterpret_cast<uintptr_t>(this) + 0x28)); }
	int get_player_id() { return *reinterpret_cast<int*>((reinterpret_cast<uintptr_t>(this) + 0x8)); }
	uintptr_t get_name_pointer() { return *reinterpret_cast<uintptr_t*>((reinterpret_cast<uintptr_t>(this) + 0xC)); }
};

uintptr_t assembly = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));

typedef player* (__cdecl* get_player)(void* instance, int name_id);
get_player get_player_by_id = (get_player)((assembly + 0x224400));

typedef player* (__cdecl* host_player)(void* Instance);
host_player get_host_player = (host_player)((assembly + 0x224370));

typedef void(__cdecl* on_game_end)(int a1, int32_t a2, char a3); 
on_game_end game_end = (on_game_end)((assembly + 0x29FDB0));

typedef void(__cdecl* exit_game)(int a1);
exit_game leave_game = (exit_game)((assembly + 0x21F1A0));

typedef int(__cdecl* get_count)(void* instance);
get_count player_count = (get_count)((assembly + 0x225730));

typedef void(__cdecl* join)(int instance); 
join on_game_join = (join)((assembly + 0x2A02D0));