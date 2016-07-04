#include "yalul.h"
#include "constant.h"

static int groupSetChild(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_GROUP_LIB));
    uiGroup *g = CAST_ARG(1, Group);

    uiGroupSetChild(g, NULL);
    yalulReleaseChild(L, g, 1);

    if(lua_gettop(L) == 2 && !lua_isnil(L, 2)) {
        uiControl *c = CAST_ARG(2, Control);
        uiGroupSetChild(g, c);
        yalulSetChild(L, g, c, 1);
    }

    RETURN_SELF;
}

static int groupSetMargined(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_GROUP_LIB));
    uiGroupSetMargined(CAST_ARG(1, Group), luaL_checkinteger(L, 2));
    RETURN_SELF;
}

static int groupMargined(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_GROUP_LIB));
    int m = uiGroupMargined(CAST_ARG(1, Group));
    lua_pushinteger(L, m);
    return 1;
}

static int groupSetTitle(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_GROUP_LIB));
    uiGroupSetTitle(CAST_ARG(1, Group), luaL_checkstring(L, 2));
    RETURN_SELF;
}

static int groupTitle(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_GROUP_LIB));
    char *text = uiGroupTitle(CAST_ARG(1, Group));
    lua_pushstring(L, text);
    uiFreeText(text);
    return 1;
}

static struct luaL_Reg meta_Group[] = {
    { "setChild",            groupSetChild },
    { "title",                groupTitle },
    { "setTitle",             groupSetTitle },
    { "setMargined",         groupSetMargined },
    { "margined",            groupMargined },

    { NULL }
};

int yalulSetGroup(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Group, 0);

    yalulSetSubControl(L, YALUL_GROUP_LIB);

    lua_pop(L, 1);
    return 0;
}
