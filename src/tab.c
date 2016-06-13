#include "yalul.h"
#include "constant.h"

static int tabMargined(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_TAB_LIB));
    int m = uiTabMargined(CAST_ARG(1, Tab), luaL_checkinteger(L, 2) - 1);
    lua_pushinteger(L, m);
    return 1;
}

static int tabSetMargined(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_TAB_LIB));
    uiTabSetMargined(CAST_ARG(1, Tab), luaL_checkinteger(L, 2) - 1, luaL_checkinteger(L, 3));
    RETURN_SELF;
}

static int tabNumPages(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_TAB_LIB));
    int m = uiTabNumPages(CAST_ARG(1, Tab));
    lua_pushinteger(L, m);
    return 1;
}

static int tabAppend(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_TAB_LIB));

    uiTab *tab = CAST_ARG(1, Tab);
    char *name = luaL_checkstring(L, 2);
    uiControl *child = CAST_ARG(3, Control);

    uiTabAppend(tab, name, child);
    yalulInsertChild(L, tab, child, -1);

    RETURN_SELF;
}

static int tabInsertAt(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_TAB_LIB));

    uiTab *tab = CAST_ARG(1, Tab);
    char *name = luaL_checkstring(L, 2);
    int index = luaL_checkinteger(L, 3);
    uiControl *child = CAST_ARG(4, Control);

    uiTabInsertAt(tab, name, index - 1, child);

    yalulInsertChild(L, tab, child, index);

    RETURN_SELF;
}

static int tabDelete(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_TAB_LIB));

    uiTab *tab = CAST_ARG(1, Tab);
    int index;
    if(lua_type(L, 2) == LUA_TUSERDATA) {
        uiControl *child = CAST_ARG(2, Control);
        index = yalulGetChildIndex(L, (uiControl *)tab, child);
    } else {
        index = luaL_checkinteger(L, 2);
    }
    assert(index > 0);

    uiTabDelete(tab, index - 1);
    yalulReleaseChild(L, tab, index);

    RETURN_SELF;
}

static struct luaL_Reg meta_Tab[] = {
    { "append",                 tabAppend },
    { "insertAt",               tabInsertAt },
    { "delete",                 tabDelete },
    { "numPages",               tabNumPages },
    { "margined",               tabSetMargined },
    { "setMargined",            tabSetMargined },

    { NULL }
};

int yalulSetTab(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Tab, 0);

    yalulSetSubControl(L, YALUL_TAB_LIB);

    lua_pop(L, 1);
    return 0;
}
