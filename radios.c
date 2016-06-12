#include "yalul.h"
#include "constant.h"

static int radioButtonsAppend(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_RADIOBUTTONS_LIB));
    uiRadioButtons *r = CAST_ARG(1, RadioButtons);
    for(int i = 2; i <= lua_gettop(L); i++) {
        uiRadioButtonsAppend(r, luaL_checkstring(L, i));
    }
    RETURN_SELF;
}

static int radioButtonsSelected(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_RADIOBUTTONS_LIB));
    int s = uiRadioButtonsSelected(CAST_ARG(1, RadioButtons));

    lua_pushinteger(L, s + 1);
    return 1;
}

static int radioButtonsSetSelected(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_RADIOBUTTONS_LIB));
    uiRadioButtonsSetSelected(CAST_ARG(1, RadioButtons),
                              luaL_checkinteger(L, 2) - 1);
    RETURN_SELF;
}

static void callback_onselected(uiRadioButtons *r, void *p)
{
    lua_State *L = (lua_State*)p;
    int s = uiRadioButtonsSelected(r);
    lua_pushinteger(L, s + 1);
    yalulDoCallback(L, r, YALUL_ONSELECTED, 1);
}

static int radioButtonsOnSelected(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_RADIOBUTTONS_LIB));
    uiRadioButtons *r = CAST_ARG(1, RadioButtons);

    yalulRegisterCallback(L, r, YALUL_ONSELECTED);

    uiRadioButtonsOnSelected(r, callback_onselected, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_RadioButtons[] = {
    { "append",                 radioButtonsAppend },
    { "selected",               radioButtonsSelected },
    { "setSelected",            radioButtonsSetSelected },
    { "onSelected",             radioButtonsOnSelected },
    { NULL }
};

int yalulSetRadioButtons(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_RadioButtons, 0);

    yalulSetSubControl(L, YALUL_RADIOBUTTONS_LIB);

    lua_pop(L, 1);
    return 0;
}
