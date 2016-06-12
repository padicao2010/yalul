#include "yalul.h"
#include "constant.h"

static int entryText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_ENTRY_LIB));
    char *text = uiEntryText(CAST_ARG(1, Entry));
    lua_pushstring(L, text);
    uiFreeText(text);
    return 1;
}

static int entrySetText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_ENTRY_LIB));
    uiEntrySetText(CAST_ARG(1, Entry), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static int entryReadOnly(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_ENTRY_LIB));
    int ro = uiEntryReadOnly(CAST_ARG(1, Entry));
    lua_pushboolean(L, ro);
    return 1;
}

static int entrySetReadOnly(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_ENTRY_LIB));
    uiEntrySetReadOnly(CAST_ARG(1, Entry), lua_toboolean(L, 2));
    RETURN_SELF;
}

static void callback_onchanged(uiEntry *entry, void *p)
{
    lua_State *L = (lua_State*)p;
    char *text = uiEntryText(entry);
    lua_pushstring(L, text);
    uiFreeText(text);
    yalulDoCallback(L, entry, YALUL_ONCHANGED, 1);
}

static int entryOnChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_ENTRY_LIB));
    uiEntry *entry = CAST_ARG(1, Entry);

    yalulRegisterCallback(L, entry, YALUL_ONCHANGED);

    uiEntryOnChanged(entry, callback_onchanged, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Entry[] = {
    { "text",                 entryText },
    { "setText",              entrySetText },
    { "readOnly",             entryReadOnly },
    { "setReadOnly",          entrySetReadOnly },
    { "onChanged",            entryOnChanged },
    { NULL }
};

int yalulSetEntry(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Entry, 0);

    yalulSetSubControl(L, YALUL_ENTRY_LIB);

    lua_pop(L, 1);
    return 0;
}
