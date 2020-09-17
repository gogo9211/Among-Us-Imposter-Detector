#pragma once
#include <iostream>
#include <WinUser.h>
#include <consoleapi.h>
#include <consoleapi2.h>
#include <consoleapi3.h>
#include <rpcndr.h>

class memory
{
	template<typename T>
	static void write_memory(void* address, const T& to_write, signed int length)
	{
		DWORD old_protection;

		VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &old_protection);

		*reinterpret_cast<byte*>(address) = to_write;

		VirtualProtect(address, length, old_protection, &old_protection);
	};

public:
	void create_console(const LPCSTR& title);
};

void memory::create_console(const LPCSTR& title)
{
	write_memory<byte>(FreeConsole, 0xC3, 1);
	AllocConsole();

	FILE* old_stream;

	auto out_console = freopen_s(&old_stream, "conout$", "w", stdout);
	auto in_console = freopen_s(&old_stream, "conin$", "r", stdin);

	::ShowWindow(GetConsoleWindow(), SW_SHOW);
	SetConsoleTitleA(title);
}
