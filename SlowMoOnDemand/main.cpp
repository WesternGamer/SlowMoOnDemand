#pragma once

#include <fstream>
#include <sstream>
#include "natives.h"
#include "enums.h"
#include "types.h"
#include "Keyboard.h"
#include "Scripthook.h"
#include "Config.h"

using namespace Keyboard;
using namespace std;

namespace SlowMoOnDemand
{
	DWORD Key = 0x59;
	float Speed = 1;
	bool AlreadyOn = false;

	/// <summary>
	/// Called every game update.
	/// </summary>
	void Update()
	{
		if (IsKeyJustUp(Key, false) && !AlreadyOn)
		{
			MISC::SET_TIME_SCALE(Speed);
			AlreadyOn = true;
			WAIT(1000);
		}
		else if (IsKeyJustUp(Key, true) && AlreadyOn)
		{
			MISC::SET_TIME_SCALE(1);
			AlreadyOn = false;
			WAIT(1000);
		}
	}

	/// <summary>
	/// This function is triggered when you interact with the game for the first time after the world loads.
	/// </summary>
	void OnWorldLoad()
	{
		srand(GetTickCount());

		ifstream myfile;

		myfile.open("SlowMoOnDemand.ini");

		char* configName = _strdup("SlowMoOnDemand.ini");
		char* topic = _strdup("SlowMoOnDemandSettings");
		char* toggleKey = _strdup("toggleKey");
		char* keyValue = _strdup("0x59");
		char* slowMoSpeed = _strdup("slowMoSpeed");
		char* speedValue = _strdup("0.40");

		if (!myfile)
		{
			Config::SaveInis(configName, topic, toggleKey, keyValue);
			Config::SaveInis(configName, topic, slowMoSpeed, speedValue);
		}

		Config::ReadInis(configName, topic, toggleKey, keyValue);

		std::stringstream ss;
		ss << std::hex << keyValue;
		ss >> Key;

		Config::ReadInis(configName, topic, slowMoSpeed, speedValue);

		Speed = std::stof(speedValue);

		delete configName;
		delete topic;
		delete toggleKey;
		delete keyValue;
		delete slowMoSpeed;
		delete speedValue;

		while (true)
		{
			Update();
			WAIT(0);
		}
	}
}

	/// <summary>
	/// Main entry point of ASI script.
	/// </summary>
	/// <param name="hInstance">Handle to DLL module.</param>
	/// <param name="reason">Reason for calling function.</param>
	/// <param name="lpReserved">Reserved.</param>
	/// <returns>True</returns>
	BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
	{
		switch (reason)
		{
		case DLL_PROCESS_ATTACH:
			scriptRegister(hInstance, SlowMoOnDemand::OnWorldLoad);
			keyboardHandlerRegister(Keyboard::OnKeyboardMessage);
			break;
		case DLL_PROCESS_DETACH:
			scriptUnregister(hInstance);
			keyboardHandlerUnregister(Keyboard::OnKeyboardMessage);
			break;
		}
		return true;
	}






