#include "yalul.h"

int controlEnable(lua_State *L)
{
    assert(yalulCheckControl(L, 1, NULL));
    uiControlEnable(CAST_ARG(1, Control));
    RETURN_SELF;
}

int controlDisable(lua_State *L)
{
    assert(yalulCheckControl(L, 1, NULL));
    uiControlDisable(CAST_ARG(1, Control));
    RETURN_SELF;
}

int controlEnabled(lua_State *L)
{
    assert(yalulCheckControl(L, 1, NULL));
    int e = uiControlEnabled(CAST_ARG(1, Control));
    lua_pushboolean(L, e);
    return 1;
}

int controlShow(lua_State *L)
{
    assert(yalulCheckControl(L, 1, NULL));
    uiControlShow(CAST_ARG(1, Control));
    RETURN_SELF;
}

int controlHide(lua_State *L)
{
    assert(yalulCheckControl(L, 1, NULL));
    uiControlHide(CAST_ARG(1, Control));
    RETURN_SELF;
}

int controlVisible(lua_State *L)
{
    assert(yalulCheckControl(L, 1, NULL));
    int v = uiControlVisible(CAST_ARG(1, Control));
    lua_pushboolean(L, v);
    return 1;
}

static struct luaL_Reg meta_Control[] = {
    { "hide",              controlHide   },
    { "show",              controlShow  },
    { "visible",           controlVisible  },

    { "enabled",           controlEnabled },
    { "disable",           controlDisable },
    { "enable",            controlEnable },

    { NULL }
};

int yalulSetControl(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Control, 0);

    return 0;
}
