#include "yalul.h"
#include "constant.h"

static int labelText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_LABEL_LIB));
    char *text = uiLabelText(CAST_ARG(1, Label));
    lua_pushstring(L, text);
    uiFreeText(text);
    return 1;
}

static int labelSetText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_LABEL_LIB));
    uiLabelSetText(CAST_ARG(1, Label), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static struct luaL_Reg meta_Label[] = {
    { "text",                 labelText },
    { "setText",              labelSetText },
    { NULL }
};

int yalulSetLabel(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Label, 0);

    yalulSetSubControl(L, YALUL_LABEL_LIB);

    lua_pop(L, 1);
    return 0;
}
