#include <windows.h>
#include <string>
#include <iostream>
#include "Structs.h"
#pragma warning(disable:4996)
#pragma comment(lib, "detours.lib")
#include "detours.h"

int IDarray[3] = { 100, 100, 100 };

Player* hook(int user_id , int unknown)
{
	Player* player = reinterpret_cast<Player*>(GetPlayerById(user_id, unknown)); //Get the player
	if (player != NULL) //Check if player is not nil
	{
	    //There are maximum of 3 imposters
		if (player->IsImposter() == 1) //Check if player is Imposter
		{
			DWORD name_array = player->GetNamePointer(); //Get his name pointer
			wchar_t* name = reinterpret_cast<wchar_t*>(name_array + 0xC); //Add 0xC to the return
			int id = player->GetID(); //Get his ID
			bool exists = std::find(std::begin(IDarray), std::end(IDarray), id) != std::end(IDarray); //Check if he is already added/printed
			if (!exists)
			{
				for (int i = 0; i <= 2; i++) //Loops the size of the array
				{
					if (IDarray[i] == 100) //Finds a place that is not taken
					{
						IDarray[i] = id; //Add his ID

						//Print the Imposter information
						std::wcout << "Impostor: " << name << " | ID: " << id << std::endl;
						std::cout << "-----------------------------" << std::endl;
						break; //Stop the loop as we already added him
					}
				}
			}
		}
	}
	return player;
}

void gameExit(int unknown)
{
	system("cls");
	for (int i = 0; i <= 2; i++) {
		IDarray[i] = 100;
	}
	return LeaveGame(unknown);
}

void GameEnding(int a1, int32_t reason, char a3)
{
	system("cls");
	for (int i = 0; i <= 2; i++) {
		IDarray[i] = 100;
	}
	return GameEnd(a1, reason, a3);
}

void main()
{
	//Create console
	AllocConsole();
	freopen("conout$", "w", stdout); 
	freopen("conin$", "r", stdin);
	SetConsoleTitleA("Among Us");

	//Hook functions
	GetPlayerById = (Player*(__cdecl*)(int, int))DetourFunction((PBYTE)(static_cast<DWORD>(Mod) + 0x224400), reinterpret_cast<PBYTE>(hook)); //Game calls this on every player once game start
	GameEnd = (void(__cdecl*)(int, int32_t, char))DetourFunction((PBYTE)(static_cast<DWORD>(Mod) + 0x29FDB0), reinterpret_cast<PBYTE>(GameEnding)); //Game calls this once round end
	LeaveGame = (void(__cdecl*)(int))DetourFunction((PBYTE)(static_cast<DWORD>(Mod) + 0x21F1A0), reinterpret_cast<PBYTE>(gameExit)); //Game calls this if you manually leave
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


/*
How the Impostor detector work:
We hook the function that game calls to get each player. This function is called on each player when the game starts. Then we get the return which is the class
and we use it to get the values.

Of course i could reverse the function args and call it myself but no need as the game got no anticheat so we can hook whatever we want
*/
