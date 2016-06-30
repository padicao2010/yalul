#include "yalul.h"
#include "constant.h"

static struct luaL_Reg meta_Separator[] = {
    { NULL }
};

int yalulSetSeparator(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Separator, 0);

    yalulSetSubControl(L, YALUL_SEPARATOR_LIB);

    lua_pop(L, 1);
    return 0;
}
