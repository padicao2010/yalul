#include "yalul.h"
#include "constant.h"

static int mlText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MULTILINEENTRY_LIB));
    char *text = uiMultilineEntryText(CAST_ARG(1, MultilineEntry));
    lua_pushstring(L, text);
    uiFreeText(text);
    return 1;
}

static int mlSetText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MULTILINEENTRY_LIB));
    uiMultilineEntrySetText(CAST_ARG(1, MultilineEntry), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static int mlReadOnly(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MULTILINEENTRY_LIB));
    int ro = uiMultilineEntryReadOnly(CAST_ARG(1, MultilineEntry));
    lua_pushboolean(L, ro);
    return 1;
}

static int mlSetReadOnly(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MULTILINEENTRY_LIB));
    uiMultilineEntrySetReadOnly(CAST_ARG(1, MultilineEntry), lua_toboolean(L, 2));
    RETURN_SELF;
}

static void callback_onchanged(uiMultilineEntry *entry, void *p)
{
    lua_State *L = (lua_State*)p;
    char *text = uiMultilineEntryText(entry);
    lua_pushstring(L, text);
    uiFreeText(text);
    yalulDoCallback(L, entry, YALUL_ONCHANGED, 1);
}

static int mlAppend(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MULTILINEENTRY_LIB));
    uiMultilineEntryAppend(CAST_ARG(1, MultilineEntry), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static int mlOnChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_MULTILINEENTRY_LIB));
    uiMultilineEntry *entry = CAST_ARG(1, MultilineEntry);

    yalulRegisterCallback(L, entry, YALUL_ONCHANGED);

    uiMultilineEntryOnChanged(entry, callback_onchanged, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_MultilineEntry[] = {
    { "text",                 mlText },
    { "setText",              mlSetText },
    { "readOnly",             mlReadOnly },
    { "setReadOnly",          mlSetReadOnly },
    { "append",               mlAppend },
    { "onChanged",            mlOnChanged },
    { NULL }
};

int yalulSetMultilineEntry(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_MultilineEntry, 0);

    yalulSetSubControl(L, YALUL_MULTILINEENTRY_LIB);

    lua_pop(L, 1);
    return 0;
}
