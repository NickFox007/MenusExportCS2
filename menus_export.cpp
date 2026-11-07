#include "menus_export.h"

#include <utility>
#include <vector>

Plugin g_Plugin;
IMenusApi* g_pMenus = nullptr;
std::vector<MenuInfo> mMenuInfos;

namespace
{
Menu* GetCurrentMenu()
{
    return mMenuInfos.empty() ? nullptr : &mMenuInfos.back().hMenu;
}
}

PLUGIN_EXPOSE(Plugin, g_Plugin);

bool Plugin::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
    PLUGIN_SAVEVARS();
    g_SMAPI->AddListener(this, this);
    return true;
}

bool Plugin::Unload(char* error, size_t maxlen)
{
    g_pMenus = nullptr;
    mMenuInfos.clear();
    return true;
}

void Plugin::AllPluginsLoaded()
{
    int ret = META_IFACE_FAILED;
    g_pMenus = static_cast<IMenusApi*>(g_SMAPI->MetaFactory(MENUS_INTERFACE, &ret, nullptr));

    if (ret != META_IFACE_OK || g_pMenus == nullptr)
    {
        g_pMenus = nullptr;
        META_CONPRINTF("[%s] Menus system plugin was not found; exported menu calls will be ignored.\n", GetLogTag());
        return;
    }

    META_CONPRINTF("[%s] Menus API found.\n", GetLogTag());
}

EXPORTDLL void MenusApi_ClosePlayerMenu(int iSlot)
{
    if (g_pMenus)
        g_pMenus->ClosePlayerMenu(iSlot);
}

EXPORTDLL bool MenusApi_IsMenuOpen(int iSlot)
{
    return g_pMenus && g_pMenus->IsMenuOpen(iSlot);
}

EXPORTDLL void MenusApi_AddItemMenu(const char* sBack, const char* sText, int iType = ITEM_DEFAULT)
{
    Menu* menu = GetCurrentMenu();
    if (g_pMenus && menu)
        g_pMenus->AddItemMenu(*menu, sBack, sText, iType);
}

EXPORTDLL void MenusApi_SetExitMenu(bool bExit)
{
    Menu* menu = GetCurrentMenu();
    if (g_pMenus && menu)
        g_pMenus->SetExitMenu(*menu, bExit);
}

EXPORTDLL void MenusApi_SetBackMenu(bool bBack)
{
    Menu* menu = GetCurrentMenu();
    if (g_pMenus && menu)
        g_pMenus->SetBackMenu(*menu, bBack);
}

EXPORTDLL void MenusApi_SetTitleMenu(const char* szTitle)
{
    Menu* menu = GetCurrentMenu();
    if (g_pMenus && menu)
        g_pMenus->SetTitleMenu(*menu, szTitle);
}

using MenuCallbackFunc2 = void (*)(const char*, const char*, int, int);

EXPORTDLL void MenusApi_SetCallback(MenuCallbackFunc2 func)
{
    Menu* menu = GetCurrentMenu();
    if (!g_pMenus || !menu)
        return;

    MenuCallbackFunc callback;
    if (func)
    {
        callback = [func](const char* szBack, const char* szFront, int iItem, int iSlot)
        {
            func(szBack, szFront, iItem, iSlot);
        };
    }
    g_pMenus->SetCallback(*menu, std::move(callback));
}

EXPORTDLL void MenusApi_DisplayPlayerMenu(int iSlot, bool bClose = true, bool bReset = true)
{
    Menu* menu = GetCurrentMenu();
    if (g_pMenus && menu)
        g_pMenus->DisplayPlayerMenu(*menu, iSlot, bClose, bReset);
}

EXPORTDLL void MenusApi_NewMenuInstance(int iSlot)
{
    mMenuInfos.push_back(MenuInfo{Menu{}, iSlot});
}

EXPORTDLL void MenusApi_Clear(int iSlot)
{
    for (auto it = mMenuInfos.end(); it != mMenuInfos.begin();)
    {
        --it;
        if (it->iSlot == iSlot)
            it = mMenuInfos.erase(it);
    }
}

const char* Plugin::GetLicense() { return "Public Domain"; }
const char* Plugin::GetVersion() { return "0.4.0"; }
const char* Plugin::GetDate() { return __DATE__; }
const char* Plugin::GetLogTag() { return "MenusExport"; }
const char* Plugin::GetAuthor() { return "Nick Fox"; }
const char* Plugin::GetDescription() { return "Menus export helper plugin"; }
const char* Plugin::GetName() { return "MenusExport"; }
const char* Plugin::GetURL() { return "https://nfdev.ru/"; }
