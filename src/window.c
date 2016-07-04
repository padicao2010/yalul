#include "yalul.h"
#include "constant.h"

static int windowSetChild(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindow *w = CAST_ARG(1, Window);

    uiWindowSetChild(w, NULL);
    yalulReleaseChild(L, w, 1);

    if(lua_gettop(L) == 2 && !lua_isnil(L, 2)) {
        uiControl *c = CAST_ARG(2, Control);
        uiWindowSetChild(w, c);
        yalulSetChild(L, w, c, 1);
    }

    RETURN_SELF;
}

static int windowSetMargined(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindowSetMargined(CAST_ARG(1, Window), luaL_checkinteger(L, 2));
    RETURN_SELF;
}

static int windowMargined(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    int m = uiWindowMargined(CAST_ARG(1, Window));
    lua_pushinteger(L, m);
    return 1;
}

static int windowSetContentSize(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindowSetContentSize(CAST_ARG(1, Window), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    RETURN_SELF;
}

static int windowContentSize(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    int w, h;
    uiWindowContentSize(CAST_ARG(1, Window), &w, &h);
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 2;
}

static void callback_onsizechanged(uiWindow *w, void *p)
{
    int ws, hs;
    lua_State *L = (lua_State*)p;
    uiWindowContentSize(w, &ws, &hs);
    lua_pushinteger(L, ws);
    lua_pushinteger(L, hs);
    yalulDoCallback(L, w, YALUL_ONSIZECHANGED, 2);
}

static int windowOnContentSizeChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1,YALUL_WINDOW_LIB));
    uiWindow *w = CAST_ARG(1, Window);

    yalulRegisterCallback(L, w, YALUL_ONSIZECHANGED);

    uiWindowOnContentSizeChanged(w, callback_onsizechanged, L);

    RETURN_SELF;
}

static int windowSetPosition(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindowSetPosition(CAST_ARG(1, Window), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    RETURN_SELF;
}

static int windowPosition(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    int x, y;
    uiWindowPosition(CAST_ARG(1, Window), &x, &y);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 2;
}

static void callback_onpositionchanged(uiWindow *w, void *p)
{
    lua_State *L = (lua_State*)p;
    int x, y;
    uiWindowPosition(w, &x, &y);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    yalulDoCallback(L, w, YALUL_ONPOSCHANGED, 2);
}

static int windowOnPositionChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1,YALUL_WINDOW_LIB));
    uiWindow *w = CAST_ARG(1, Window);

    yalulRegisterCallback(L, w, YALUL_ONPOSCHANGED);

    uiWindowOnPositionChanged(w, callback_onpositionchanged, L);

    RETURN_SELF;
}

static int windowSetFullscreen(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindowSetFullscreen(CAST_ARG(1, Window), lua_toboolean(L, 2));
    RETURN_SELF;
}

static int windowFullscreen(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    int b = uiWindowFullscreen(CAST_ARG(1, Window));
    lua_pushboolean(L, b);
    return 1;
}

static int windowSetBorderless(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindowSetBorderless(CAST_ARG(1, Window), lua_toboolean(L, 2));
    RETURN_SELF;
}

static int windowBorderless(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    int b = uiWindowBorderless(CAST_ARG(1, Window));
    lua_pushboolean(L, b);
    return 1;
}

static int windowSetTitle(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    uiWindowSetTitle(CAST_ARG(1, Window), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static int windowTitle(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));
    char *title = uiWindowTitle(CAST_ARG(1, Window));
    lua_pushstring(L, title);
    uiFreeText(title);
    return 1;
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
    { "margined",            windowMargined },
    { "title",               windowTitle },
    { "setTitle",            windowTitle },
    { "position",            windowPosition },
    { "setPosition",         windowSetPosition },
    { "onPositionChanged",   windowOnPositionChanged },
    { "contentSize",         windowContentSize },
    { "setContentSize",      windowSetContentSize },
    { "onContentSizeChanged",   windowOnContentSizeChanged },
    { "fullscreen",          windowFullscreen },
    { "setFullscreen",       windowSetFullscreen },
    { "borderless",          windowBorderless },
    { "setBorderless",       windowSetBorderless },

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
