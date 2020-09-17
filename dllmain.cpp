#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <array>
#include "Structs.h"
#include "detours.h"
#include "memory.h"
#include "ConsoleColors.h"
#pragma warning(disable:4996)
#pragma comment(lib, "detours.lib")

using namespace std; 
uintptr_t id_array[3] = { 100, 100, 100 };
uintptr_t player_display = NULL;

player* initialize(void* class_pointer, uintptr_t user_id)
{
	player* get_player = reinterpret_cast<player*>(get_player_by_id(class_pointer, user_id)); //Get the player
	if (get_player != NULL) //Check if player is not nil
	{
		//There are maximum of 3 imposters
		if (get_player->is_imposter() == 1) //Check if player is Imposter
		{
			//Display the amount of players game start with
			if (player_display == NULL)
			{
				uintptr_t count = player_count(class_pointer);
				player* host_player = get_host_player(class_pointer);
				uintptr_t host_player_array = host_player->get_name_pointer();
				wchar_t* host_name_player = reinterpret_cast<wchar_t*>(host_player_array + 0xC);
				cout << white << "[" << cyan << "x" << white << "] " << "Players: " << count << "\r\n";
				cout << white << "[" << iyellow << "x" << white << "] " << "Host Player Name: ";
				wcout << host_name_player << "\n\n";
				player_display = 1;
			}

			DWORD name_array = get_player->get_name_pointer(); //Get name pointers
			wchar_t* name = reinterpret_cast<wchar_t*>(name_array + 0xC); //Add 0xC to the return
			uintptr_t id_player = get_player->get_player_id(); //Get player id
			bool name_exist = find(begin(id_array), end(id_array), id_player) != end(id_array); //Check if he is already added/printed
			if (!name_exist)
			{
				for (int i = 0; i <= 2; ++i) //Loops the size of the array
				{
					if (id_array[i] == 100) //Finds a place that is not taken
					{
						id_array[i] = id_player; //Add player ID

						//Print the Imposter information
						cout << white << "[" << red << "x" << white << "] " << "Impostor: ";
						wcout << name;
						cout << " | ID: " << id_player << "\n";

						cout << "----------------------------------" << endl;
						break; //Stop the loop as we already added him
					}
				}
			}
		}
	}
	return get_player;
}

void game_exit(int process)
{
	system("CLS");;
	for (int i = 0; i <= 2; ++i)
	{
		id_array[i] = 100;
	}
	player_display = 0;
	return leave_game(process);
}

void game_ending(int a1, int32_t a2, char a3)
{
	system("CLS");
	for (int i = 0; i <= 2; ++i) 
	{
		id_array[i] = 100;
	}
	player_display = 0;
	return game_end(a1, a2, a3);
}

void game_join(int process)
{
	system("cls");
	for (int i = 0; i <= 2; ++i) 
	{
		id_array[i] = 100;
	}
	player_display = 0;
	return on_game_join(process);
}

void create_console(const char* title)
{
    memory create;
    create.create_console(title);
}

void entry_point()
{
    create_console("Among Us Imposter Finder by gogo#0001 & Gian#6762 & Cube#9709");
	get_player_by_id = (player* (__cdecl*)(void*, int))DetourFunction((PBYTE)(static_cast<DWORD>(assembly) + 0x224400), reinterpret_cast<PBYTE>(initialize)); //Game calls this on every player once game start
	game_end = (void(__cdecl*)(int, int32_t, char))DetourFunction((PBYTE)(static_cast<DWORD>(assembly) + 0x29FDB0), reinterpret_cast<PBYTE>(game_ending)); //Game calls this once round end
	leave_game = (void(__cdecl*)(int))DetourFunction((PBYTE)(static_cast<DWORD>(assembly) + 0x21F1A0), reinterpret_cast<PBYTE>(game_exit)); //Game calls this if you manually leave
	on_game_join = (void(__cdecl*)(int))DetourFunction((PBYTE)(static_cast<DWORD>(assembly) + 0x2A02D0), reinterpret_cast<PBYTE>(game_join)); //Game calls this when you join a game
	while (true)
	{
		std::string input;
		std::cin >> input;
		if (input == "clear" || input == "Clear")
		{
			system("CLS");
			for (int i = 0; i <= 2; ++i) 
			{
				id_array[i] = 100;
			}
			player_display = 0;
		}
	}
}

extern "C" BOOL WINAPI DllMain(
    HINSTANCE const instance,
    DWORD     const reason,
    LPVOID    const reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_point), nullptr, NULL, nullptr);
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
