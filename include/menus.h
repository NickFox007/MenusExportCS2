#pragma once

#include <functional>
#include <string>

#define Menus_INTERFACE "IMenusApi"

#define ITEM_HIDE 0
#define ITEM_DEFAULT 1
#define ITEM_DISABLED 2

typedef std::function<void(const char* szBack, const char* szFront, int iItem, int iSlot)> MenuCallbackFunc;

struct Items
{
    int iType;
    std::string sBack;
    std::string sText;
};

struct Menu
{
    std::string szTitle;	
    std::vector<Items> hItems;
    bool bBack;
    bool bExit;
	MenuCallbackFunc hFunc;
};

struct MenuPlayer
{
    bool bEnabled;
    int iList;
    Menu hMenu;
};

class IMenusApi
{
public:
	virtual void AddItemMenu(Menu& hMenu, const char* sBack, const char* sText, int iType = 1) = 0;
	virtual void DisplayPlayerMenu(Menu& hMenu, int iSlot, bool bClose = true) = 0;
	virtual void SetExitMenu(Menu& hMenu, bool bExit) = 0;
	virtual void SetBackMenu(Menu& hMenu, bool bBack) = 0;
	virtual void SetTitleMenu(Menu& hMenu, const char* szTitle) = 0;
	virtual void SetCallback(Menu& hMenu, MenuCallbackFunc func) = 0;
    virtual void ClosePlayerMenu(int iSlot) = 0;
    virtual std::string escapeString(const std::string& input) = 0;
    virtual bool IsMenuOpen(int iSlot) = 0;
	virtual void DisplayPlayerMenu(Menu& hMenu, int iSlot, bool bClose = true, bool bReset = true) = 0;
};

struct MenuInfo
{
    Menu hMenu;
    int iSlot;
};
