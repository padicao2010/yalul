#include "yalul.h"
#include "constant.h"

static int colorButtonColor(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_COLORBUTTON_LIB));
    double r, g, b, a;
    uiColorButtonColor(CAST_ARG(1, ColorButton), &r, &g, &b, &a);
    lua_pushnumber(L, r);
    lua_pushnumber(L, g);
    lua_pushnumber(L, b);
    lua_pushnumber(L, a);

    return 4;
}

static int colorButtonSetColor(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_COLORBUTTON_LIB));
    uiColorButtonSetColor(CAST_ARG(1, ColorButton),
                          luaL_checknumber(L, 2),
                          luaL_checknumber(L, 3),
                          luaL_checknumber(L, 4),
                          luaL_checknumber(L, 5));
    RETURN_SELF;
}

static void callback_onchanged(uiColorButton *cb, void *p)
{
    lua_State *L = (lua_State*)p;
    double r, g, b, a;
    uiColorButtonColor(cb, &r, &g, &b, &a);
    lua_pushnumber(L, r);
    lua_pushnumber(L, g);
    lua_pushnumber(L, b);
    lua_pushnumber(L, a);
    yalulDoCallback(L, cb, YALUL_ONCHANGED, 4);
}

static int colorButtonOnChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_COLORBUTTON_LIB));
    uiColorButton *cb = CAST_ARG(1, ColorButton);

    yalulRegisterCallback(L, cb, YALUL_ONCHANGED);

    uiColorButtonOnChanged(cb, callback_onchanged, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_ColorButton[] = {
    { "color",                 colorButtonColor },
    { "setColor",              colorButtonSetColor },
    { "onChanged",             colorButtonOnChanged },
    { NULL }
};

int yalulSetColorButton(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_ColorButton, 0);

    yalulSetSubControl(L, YALUL_COLORBUTTON_LIB);

    lua_pop(L, 1);
    return 0;
}
