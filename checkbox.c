#include "yalul.h"
#include "constant.h"

static int checkboxText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_CHECKBOX_LIB));
    char *text = uiCheckboxText(CAST_ARG(1, Checkbox));
    lua_pushstring(L, text);
    uiFreeText(text);
    return 1;
}

static int checkboxSetText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_CHECKBOX_LIB));
    uiCheckboxSetText(CAST_ARG(1, Checkbox), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static int checkboxChecked(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_CHECKBOX_LIB));
    int c = uiCheckboxChecked(CAST_ARG(1, Checkbox));
    lua_pushboolean(L, c);
    return 1;
}

static int checkboxSetChecked(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_CHECKBOX_LIB));
    uiCheckboxSetChecked(CAST_ARG(1, Checkbox), lua_toboolean(L, 2));
    RETURN_SELF;
}

static void callback_ontoggled(uiCheckbox *cb, void *p)
{
    lua_State *L = (lua_State*)p;
    int checked = uiCheckboxChecked(cb);
    lua_pushboolean(L, checked);
    yalulDoCallback(L, cb, YALUL_ONTOGGLED, 1);
}

static int checkboxOnToggled(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_CHECKBOX_LIB));
    uiCheckbox *cb = CAST_ARG(1, Checkbox);

    yalulRegisterCallback(L, cb, YALUL_ONTOGGLED);

    uiCheckboxOnToggled(cb, callback_ontoggled, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Checkbox[] = {
    { "text",                 checkboxText },
    { "setText",              checkboxSetText },
    { "checked",              checkboxChecked },
    { "setChecked",           checkboxSetChecked },
    { "onToggled",            checkboxOnToggled },
    { NULL }
};

int yalulSetCheckbox(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Checkbox, 0);

    yalulSetSubControl(L, YALUL_CHECKBOX_LIB);

    lua_pop(L, 1);
    return 0;
}
