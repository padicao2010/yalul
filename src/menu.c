#include "yalul.h"
#include "constant.h"

static int menuAppendItem(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));

    uiMenu *menu = CAST_ARG(1, Menu);
    uiMenuItem *item = uiMenuAppendItem(menu, luaL_checkstring(L, 2));
    yalulInsertChild(L, menu, item, -1);

    RETURN_SELF;
}

static int menuAppendCheckItem(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));

    uiMenu *menu = CAST_ARG(1, Menu);
    uiMenuItem *item = uiMenuAppendCheckItem(menu, luaL_checkstring(L, 2));
    yalulInsertChild(L, menu, item, -1);

    RETURN_SELF;
}

static int menuAppendQuitItem(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));

    uiMenu *menu = CAST_ARG(1, Menu);
    uiMenuItem *item = uiMenuAppendQuitItem(menu);
    yalulInsertChild(L, menu, item, -1);

    RETURN_SELF;
}

static int menuAppendAboutItem(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));

    uiMenu *menu = CAST_ARG(1, Menu);
    uiMenuItem *item = uiMenuAppendAboutItem(menu);
    yalulInsertChild(L, menu, item, -1);

    RETURN_SELF;
}

static int menuAppendPreferencesItem(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));

    uiMenu *menu = CAST_ARG(1, Menu);
    uiMenuItem *item = uiMenuAppendPreferencesItem(menu);
    yalulInsertChild(L, menu, item, -1);

    RETURN_SELF;
}

static int menuAppendSeparator(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));

    uiMenuAppendSeparator(CAST_ARG(1, Menu));

    RETURN_SELF;
}

static uiMenuItem *getMenuItem(lua_State *L, uiMenu *m, int index)
{
    lua_pushlightuserdata(L, m);
    lua_gettable(L, LUA_REGISTRYINDEX);

    if(index <= 0) {
        lua_len(L, -1);
    } else {
        lua_pushinteger(L, index);
    }

    lua_gettable(L, -2);
    uiMenuItem *item = (uiMenuItem *)lua_topointer(L, -1);
    lua_pop(L, 2);
    assert(item);
    return item;
}

static int menuEnableItem(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));
    uiMenu *menu = CAST_ARG(1, Menu);

    int index;
    if(lua_gettop(L) == 2) {
        index = luaL_checkinteger(L, 2);
    } else {
        index = -1;
    }
    uiMenuItem *item = getMenuItem(L, menu, index);

    uiMenuItemEnable(item);

    RETURN_SELF;
}

static int menuDisableItem(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));
    uiMenu *menu = CAST_ARG(1, Menu);

    int index;
    if(lua_gettop(L) == 2) {
        index = luaL_checkinteger(L, 2);
    } else {
        index = -1;
    }
    uiMenuItem *item = getMenuItem(L, menu, index);

    uiMenuItemDisable(item);

    RETURN_SELF;
}

static int menuItemChecked(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));
    uiMenu *menu = CAST_ARG(1, Menu);

    uiMenuItem *item = getMenuItem(L, menu, luaL_checkinteger(L, 2));

    lua_pushboolean(L, uiMenuItemChecked(item));

    return 1;
}

static int menuSetItemChecked(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));
    uiMenu *menu = CAST_ARG(1, Menu);

    int index;
    int checked;
    if(lua_gettop(L) == 3) {
        index = luaL_checkinteger(L, 2);
        checked = lua_toboolean(L, 3);
    } else {
        index = -1;
        checked = lua_toboolean(L, 2);
    }
    uiMenuItem *item = getMenuItem(L, menu, index);

    uiMenuItemSetChecked(item, checked);

    RETURN_SELF;
}

static void callback_onclicked(uiMenuItem *bt, uiWindow *w, void *p)
{
    lua_State *L = (lua_State*)p;

    int checked = uiMenuItemChecked(bt);
    lua_pushboolean(L, checked);
    yalulDoCallback(L, bt, YALUL_ONCLICKED, 1);
}

static int menuOnItemClicked(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MENU_LIB));
    uiMenu *menu = CAST_ARG(1, Menu);

    int index;
    if(lua_gettop(L) == 3) {
        index = luaL_checkinteger(L, 2);
    } else {
        index = -1;
    }
    uiMenuItem *item = getMenuItem(L, menu, index);

    yalulRegisterCallback(L, item, YALUL_ONCLICKED);
    uiMenuItemOnClicked(item, callback_onclicked, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Menu[] = {
    { "appendItem",               menuAppendItem },
    { "appendCheckItem",          menuAppendCheckItem },
    { "appendQuitItem",           menuAppendQuitItem },
    { "appendPreferencesItem",    menuAppendPreferencesItem },
    { "appendAboutItem",          menuAppendAboutItem },
    { "appendSeparator",          menuAppendSeparator },
    { "enableItem",               menuEnableItem },
    { "disableItem",              menuDisableItem },
    { "onItemClicked",            menuOnItemClicked },
    { "itemChecked",              menuItemChecked },
    { "setItemChecked",           menuSetItemChecked },
    { NULL }
};

int yalulSetMenu(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Menu, 0);

    yalulInitMenu(L, YALUL_MENU_LIB);

    lua_pop(L, 1);
    return 0;
}
