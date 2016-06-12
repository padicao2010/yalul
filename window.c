#include "yalul.h"
#include "constant.h"

static int windowSetChild(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindow *w = CAST_ARG(1, Window);
    uiControl *c = CAST_ARG(2, Control);

    uiWindowSetChild(w, c);

    yalulReleaseChild(L, w, 1);

    yalulSetChild(L, w, c, 1);

    RETURN_SELF;
}

static int windowSetMargined(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindowSetMargined(CAST_ARG(1, Window), luaL_checknumber(L, 2));
    RETURN_SELF;
}

static int callback_onclose(uiWindow *w, void *p)
{
    return yalulDoCallback((lua_State*)p, w, YALUL_ONCLOSING, 0);
}

static int windowOnClosing(lua_State *L)
{
    assert(yalulCheckControl(L, 1,YALUL_WINDOW_LIB));
    uiWindow *w = CAST_ARG(1, Window);

    yalulRegisterCallback(L, w, YALUL_ONCLOSING);

    uiWindowOnClosing(w, callback_onclose, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Window[] = {
    { "setChild",            windowSetChild },
    { "setMargined",         windowSetMargined },
    { "onClosing",           windowOnClosing },

    { NULL }
};

int yalulSetWindow(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Window, 0);

    yalulSetSubControl(L, YALUL_WINDOW_LIB);

    lua_pop(L, 1);
    return 0;
}
