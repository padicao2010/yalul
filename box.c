#include "yalul.h"
#include "constant.h"

static int boxAppend(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_BOX_LIB));

    uiBox *box = CAST_ARG(1, Box);
    uiControl *child = CAST_ARG(2, Control);
    int stretchy = 0;
    if(lua_gettop(L) > 2) {
        stretchy = lua_toboolean(L, 3);
    }
    uiBoxAppend(box, child, stretchy);

    yalulSetChild(L, box, child, -1);

    RETURN_SELF;
}

static int boxDelete(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_BOX_LIB));

    uiBox *box = CAST_ARG(1, Box);
    int index;
    if(lua_type(L, 2) == LUA_TUSERDATA) {
        uiControl *child = CAST_ARG(2, Control);
        index = yalulGetChildIndex(L, (uiControl *)box, child);
    } else {
        index = luaL_checkinteger(L, 2);
    }
    assert(index > 0);

    uiBoxDelete(box, index - 1);
    yalulReleaseChild(L, box, index);

    RETURN_SELF;
}

static int boxSetPadded(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_BOX_LIB));
    uiBoxSetPadded(CAST_ARG(1, Box), luaL_checkinteger(L, 2));
    RETURN_SELF;
}

static int boxPadded(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_BOX_LIB));
    int padded = uiBoxPadded(CAST_ARG(1, Box));
    lua_pushinteger(L, padded);
    return 1;
}

static struct luaL_Reg meta_Box[] = {
    { "append",                 boxAppend },
    { "delete",                 boxDelete},
    { "setPadded",              boxSetPadded },
    { "Padded",                 boxPadded},
    { NULL }
};

int yalulSetBox(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Box, 0);

    yalulSetSubControl(L, YALUL_BOX_LIB);

    lua_pop(L, 1);
    return 0;
}
