#include "yalul.h"
#include "constant.h"

static int buttonText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_BUTTON_LIB));
    char *text = uiButtonText(CAST_ARG(1, Button));
    lua_pushstring(L, text);
    uiFreeText(text);
    return 1;
}

static int buttonSetText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_BUTTON_LIB));
    uiButtonSetText(CAST_ARG(1, Button), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static void callback_onclicked(uiButton *bt, void *p)
{
    lua_State *L = (lua_State*)p;

    yalulDoCallback(L, bt, YALUL_ONCLICKED, 0);
}

static int buttonOnClicked(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_BUTTON_LIB));
    uiButton *bt = CAST_ARG(1, Button);

    yalulRegisterCallback(L, bt, YALUL_ONCLICKED);

    uiButtonOnClicked(bt, callback_onclicked, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Button[] = {
    { "text",                 buttonText },
    { "setText",              buttonSetText },
    { "onClicked",            buttonOnClicked },
    { NULL }
};

int yalulSetButton(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Button, 0);

    yalulSetSubControl(L, YALUL_BUTTON_LIB);

    lua_pop(L, 1);
    return 0;
}
