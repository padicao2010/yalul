#include "yalul.h"
#include "constant.h"

static int comboboxAppend(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_COMBOBOX_LIB));
    uiCombobox *c = CAST_ARG(1, Combobox);
    for(int i = 2; i <= lua_gettop(L); i++) {
        uiComboboxAppend(c, luaL_checkstring(L, i));
    }
    RETURN_SELF;
}

static int comboboxSelected(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_COMBOBOX_LIB));
    int s = uiComboboxSelected(CAST_ARG(1, Combobox));

    lua_pushinteger(L, s + 1);
    return 1;
}

static int comboboxSetSelected(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_COMBOBOX_LIB));
    uiComboboxSetSelected(CAST_ARG(1, Combobox),
                              luaL_checkinteger(L, 2) - 1);
    RETURN_SELF;
}

static void callback_onselected(uiCombobox *c, void *p)
{
    lua_State *L = (lua_State*)p;
    int s = uiComboboxSelected(c);
    lua_pushinteger(L, s + 1);
    yalulDoCallback(L, c, YALUL_ONSELECTED, 1);
}

static int comboboxOnSelected(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_COMBOBOX_LIB));
    uiCombobox *c = CAST_ARG(1, Combobox);

    yalulRegisterCallback(L, c, YALUL_ONSELECTED);

    uiComboboxOnSelected(c, callback_onselected, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Combobox[] = {
    { "append",                 comboboxAppend },
    { "selected",               comboboxSelected },
    { "setSelected",            comboboxSetSelected },
    { "onSelected",             comboboxOnSelected },
    { NULL }
};

int yalulSetCombobox(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Combobox, 0);

    yalulSetSubControl(L, YALUL_COMBOBOX_LIB);

    lua_pop(L, 1);
    return 0;
}
