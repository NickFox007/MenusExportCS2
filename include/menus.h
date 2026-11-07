#pragma once

#include <functional>
#include <string>
#include <vector>

#define MENUS_INTERFACE "IMenusApi"

#define ITEM_HIDE 0
#define ITEM_DEFAULT 1
#define ITEM_DISABLED 2

enum class ItemKind : int
{
    BUTTON = 0,
    TOGGLE = 1,
    SELECT = 2,
};

using MenuCallbackFunc = std::function<void(const char* szBack, const char* szFront, int iItem, int iSlot)>;
using MenuToggleCallbackFunc = std::function<void(const char* szBack, bool bState, int iItem, int iSlot)>;
using MenuSelectCallbackFunc = std::function<void(const char* szBack, const char* szOptionBack, int iOption, int iItem, int iSlot)>;

struct SelectOption
{
    std::string sBack;
    std::string sText;
};

struct Menu;

struct ItemRef
{
    Menu* pMenu = nullptr;
    int iIndex = -1;
};

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
    bool bBack = false;
    bool bExit = false;
    MenuCallbackFunc hFunc = nullptr;

    void clear()
    {
        szTitle.clear();
        hItems.clear();
        bBack = false;
        bExit = false;
        hFunc = nullptr;
    }
};

struct MenuPlayer
{
    bool bEnabled;
    int iList;
    Menu hMenu;
    int iEnd;

    void clear()
    {
        bEnabled = false;
        iList = 0;
        hMenu.clear();
        iEnd = 0;
    }
};

enum class MenuType : int
{
    CHAT = 0,
    CENTER = 1,
    CENTER_WASD = 2,
    HUD_LAYOUT = 3,
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
    virtual void DisplayPlayerMenu(Menu& hMenu, int iSlot, bool bClose, bool bReset) = 0;
    virtual void AddRawItemMenu(Menu& hMenu, const char* sBack, const char* sText, int iType = 1) = 0;
    virtual MenuType GetMenuType(int iSlot) = 0;
    virtual void SetDescriptionMenu(Menu& hMenu, const char* szDescription) = 0;
    virtual void AddToggleMenu(Menu& hMenu, const char* sBack, const char* sText, bool bDefault = false, MenuToggleCallbackFunc func = nullptr, int iType = 1) = 0;
    virtual ItemRef AddSelectMenu(Menu& hMenu, const char* sBack, const char* sText, int iDefault = 0, MenuSelectCallbackFunc func = nullptr, int iType = 1) = 0;
    virtual void AddSelectOption(ItemRef hItem, const char* sOptionBack, const char* sOptionText) = 0;
    virtual void SetToggleState(ItemRef hItem, bool bState) = 0;
    virtual bool GetToggleState(ItemRef hItem) = 0;
    virtual void SetSelectedOption(ItemRef hItem, int iOption) = 0;
    virtual int GetSelectedOption(ItemRef hItem) = 0;
};

struct MenuInfo
{
    Menu hMenu;
    int iSlot;
};
