#include "yalul.h"
#include "constant.h"

static int spinboxValue(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_SPINBOX_LIB));
    int v = uiSpinboxValue(CAST_ARG(1, Spinbox));
    lua_pushinteger(L, v);
    return 1;
}

static int spinboxSetValue(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_SPINBOX_LIB));
    uiSpinboxSetValue(CAST_ARG(1, Spinbox), luaL_checkinteger(L, 2));
    RETURN_SELF;
}

static void callback_onchanged(uiSpinbox *sb, void *p)
{
    lua_State *L = (lua_State*)p;
    int v = uiSpinboxValue(sb);
    lua_pushinteger(L, v);
    yalulDoCallback(L, sb, YALUL_ONCHANGED, 1);
}

static int spinboxOnChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_SPINBOX_LIB));
    uiSpinbox *sb = CAST_ARG(1, Spinbox);

    yalulRegisterCallback(L, sb, YALUL_ONCHANGED);

    uiSpinboxOnChanged(sb, callback_onchanged, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Spinbox[] = {
    { "value",                 spinboxValue },
    { "setValue",              spinboxSetValue },
    { "onChanged",             spinboxOnChanged },
    { NULL }
};

int yalulSetSpinbox(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Spinbox, 0);

    yalulSetSubControl(L, YALUL_SPINBOX_LIB);

    lua_pop(L, 1);
    return 0;
}
