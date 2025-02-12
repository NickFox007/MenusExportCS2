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
IVEngineServer *engine = NULL;
ICvar *icvar = NULL;
IMenusApi* g_pMenus;

CGlobalVars *GetGameGlobals()
{
	INetworkGameServer *server = g_pNetworkServerService->GetIGameServer();

	if(!server)
		return nullptr;

	return g_pNetworkServerService->GetIGameServer()->GetGlobals();
}


PLUGIN_EXPOSE(Plugin, g_Plugin);
bool Plugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);

	// Required to get the IMetamodListener events
	g_SMAPI->AddListener( this, this );
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
		V_strncpy(text, "\nMissing Menus system plugin. Unloading...\n", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), text);
		std::string sBuffer = "meta unload "+std::to_string(g_PLID);
		engine->ServerCommand(sBuffer.c_str());
		return;
	}
	else
	{
		V_strncpy(text, "\n==========================\n\n\n   MenusApi found!\n\n\n==========================\n", 64);
		ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), text);
	}
}

EXPORTDLL void MenusApi_ClosePlayerMenu(int iSlot)
{
	//char text[64];
	//V_strncpy(text, "Closing active menu...", 64);
	//ConColorMsg(Color(255, 0, 0, 255), "[MenusExport] %s (%i)\n", text, iSlot);
	g_pMenus->ClosePlayerMenu(iSlot);
}

EXPORTDLL bool MenusApi_IsMenuOpen(int iSlot)
{
	//char text[64];
	//V_strncpy(text, "Getting is menu opened...", 64);
	//ConColorMsg(Color(255, 0, 0, 255), "[MenusExport] %s (%i)\n", text, iSlot);
	return g_pMenus->IsMenuOpen(iSlot);	
}


const char *Plugin::GetLicense()
{
	return "Public Domain";
}

const char *Plugin::GetVersion()
{
	return "0.2";
}

const char *Plugin::GetDate()
{
	return __DATE__;
}

const char *Plugin::GetLogTag()
{
	return "MenusExport";
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
	return "MenusExport";
}

const char *Plugin::GetURL()
{
	return "https://nfdev.ru/";
}
