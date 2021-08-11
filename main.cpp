#include <iostream>
#include <Windows.h>
#include <shlwapi.h>
	
#include <../steam/steamtypes.h>
#include <../steam/steam_api.h>

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "steam_api.lib")

#define _CRT_SECURE_NO_WARNINGS	

int main()
{
	wchar_t installFolder[MAX_PATH] = { 0 };
	unsigned long bufferLength = sizeof(installFolder);
	unsigned long type = REG_SZ;

	std::cout << "[+] Start\n";

	if (SHGetValue(HKEY_CURRENT_USER, TEXT("Software\\Valve\\Steam"), TEXT("SteamPath"), &type, installFolder, &bufferLength) != ERROR_SUCCESS)
	{
		std::wcout << L"[-] Could not find Steam Install directory in : HKEY_CURRENT_USER\\Software\\Valve\\Steam\\SteamPath)\n";
		exit(1);
	}

	std::wcout << L"[+] Steam install folder found : " << installFolder << std::endl;

	if (!SetDllDirectory(installFolder))
	{
		std::wcout << L"[-] SetDllDirectory failed\n";
		exit(2);
	}

	std::wcout << L"[+] SetDllDirectory : " << installFolder << std::endl;

	HMODULE steam = LoadLibrary(L"steam");
	HMODULE steamapi = LoadLibrary(L"steam_api");
	HMODULE steamclientdll = LoadLibrary(L"steamclient");

	if (!steam || !steamapi || !steamclientdll)
	{
		std::wcout << L"[-] Couldn't load : ";
		if (!steam)
			std::wcout << L"Steam.dll ";

		if (!steamapi)
			std::wcout << L"steam_api.dll ";

		if(!steamclientdll)
			std::wcout << L"steam_api.dll ";
		exit(3);
	}

	std::wcout << L"[+] Libraries loaded\n";

	if (!SteamAPI_Init())
	{
		std::wcout << L"[-] SteamAPI failed to load\n";
		exit(4);
	};

	std::wcout << L"[+] SteamAPI started successfully\n";

	unsigned char steam3Cookie[2048];
	uint32 steam3CookieLen;
	SteamUser()->GetAuthSessionTicket(steam3Cookie, sizeof(steam3Cookie), &steam3CookieLen);

	std::cout << "[+] steam3Cookie : ";
	for (int i = 0; i < steam3CookieLen; i++)
		printf("%02x\n", steam3Cookie[i]);

	std::cout << "\n[+] Cookie length : " << std::dec << steam3CookieLen << " octets\n";

}