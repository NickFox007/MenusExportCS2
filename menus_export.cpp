/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Sample Plugin
 * Written by AlliedModders LLC.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 *
 * This sample plugin is public domain.
 */

#include <stdio.h>
#include "menus_export.h"
#include "iserver.h"

Plugin g_Plugin;
IServerGameDLL *server = NULL;
IServerGameClients *gameclients = NULL;
IVEngineServer *engine = NULL;
//IGameEventManager2 *gameevents = NULL;
ICvar *icvar = NULL;
IMenusApi* g_pMenus;

// Should only be called within the active game loop (i e map should be loaded and active)
// otherwise that'll be nullptr!
CGlobalVars *GetGameGlobals()
{
	INetworkGameServer *server = g_pNetworkServerService->GetIGameServer();

	if(!server)
		return nullptr;

	return g_pNetworkServerService->GetIGameServer()->GetGlobals();
}

//#if 0
// Currently unavailable, requires hl2sdk work!
//ConVar sample_cvar("sample_cvar", "42", 0);
//#endif

//CON_COMMAND_F(sample_command, "Sample command", FCVAR_NONE)
//{
	//META_CONPRINTF( "Sample command called by %d. Command: %s\n", context.GetPlayerSlot(), args.GetCommandString() );
/*
CON_COMMAND_F(play_em, "Emit sound for player", FCVAR_NONE)
{
	META_CONPRINT("Begin...\n");
	if (args.ArgC() == 4 && args[1][0])
	{
		
		if(!containsOnlyDigits(args[1]))
			META_CONPRINT("Invalid usage! Player slot value should contain only digits");
		else
		{		
			
			int iSlot = std::stoll(args[1]);
			
			CCSPlayerController* pController = CCSPlayerController::FromSlot(iSlot);
			if (!pController)
				META_CONPRINTF("Player with index %d wasn't found", iSlot);
			else
			{
				META_CONPRINT("Correct usage!");
			}
			
			META_CONPRINT("Correct usage!");
		}
		
		META_CONPRINT("Correct usage!\n");
	}
	else
		META_CONPRINT("Usage: emitsound <slot> <volume> <path>\n");
}*/

PLUGIN_EXPOSE(Plugin, g_Plugin);
bool Plugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, icvar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_ANY(GetServerFactory, server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_ANY(GetServerFactory, gameclients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	GET_V_IFACE_ANY(GetEngineFactory, g_pNetworkServerService, INetworkServerService, NETWORKSERVERSERVICE_INTERFACE_VERSION);

	// Currently doesn't work from within mm side, use GetGameGlobals() in the mean time instead
	// gpGlobals = ismm->GetCGlobals();

	// Required to get the IMetamodListener events
	g_SMAPI->AddListener( this, this );

	//META_CONPRINTF( "Starting plugin.\n" );



	META_CONPRINTF( "All hooks started!\n" );

	g_pCVar = icvar;
	ConVar_Register( FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL );

	return true;
}

bool Plugin::Unload(char *error, size_t maxlen)
{

	return true;
}

void Plugin::AllPluginsLoaded()
{
	/* This is where we'd do stuff that relies on the mod or other plugins 
	 * being initialized (for example, cvars added and events registered).
	 */
	int ret;
	char text[64];
	g_pMenus = (IMenusApi *)g_SMAPI->MetaFactory(Menus_INTERFACE, &ret, NULL);

	if (ret == META_IFACE_FAILED)
	{
		V_strncpy(text, "Missing Menus system plugin. Unloading...", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), text);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	else
	{
		V_strncpy(text, "\n==========================\n\n\n   MenusApi found!\n\n\n==========================", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), text);
	}
}

EXPORTDLL void MenusApi_ClosePlayerMenu(int iSlot)
{
	char text[64];
	V_strncpy(text, "Closing active menu...", 64);
	ConColorMsg(Color(255, 0, 0, 255), "[MenusExport] %s (%i)\n", text, iSlot);
	g_pMenus->ClosePlayerMenu(iSlot);
}

EXPORTDLL bool MenusApi_IsMenuOpen(int iSlot)
{
	char text[64];
	V_strncpy(text, "Getting is menu opened...", 64);
	ConColorMsg(Color(255, 0, 0, 255), "[MenusExport] %s (%i)\n", text, iSlot);
	return g_pMenus->IsMenuOpen(iSlot);
	//return true;
}


const char *Plugin::GetLicense()
{
	return "Public Domain";
}

const char *Plugin::GetVersion()
{
	return "0.1";
}

const char *Plugin::GetDate()
{
	return __DATE__;
}

const char *Plugin::GetLogTag()
{
	return "MenusSharp";
}

const char *Plugin::GetAuthor()
{
	return "Nick Fox";
}

const char *Plugin::GetDescription()
{
	return "Menus export helper plugin";
}

const char *Plugin::GetName()
{
	return "Menus Export";
}

const char *Plugin::GetURL()
{
	return "https://nfdev.ru/";
}
