#include "yalul.h"
#include "constant.h"

static int progressBarSetValue(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_PROGRESSBAR_LIB));
    int prog = luaL_checknumber(L, 2);
    assert(prog >= 0 && prog <= 100);
    uiProgressBarSetValue(CAST_ARG(1, ProgressBar), prog);
    RETURN_SELF;
}

static struct luaL_Reg meta_ProgressBar[] = {
    { "setValue",                 progressBarSetValue },
    { NULL }
};

int yalulSetProgressBar(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_ProgressBar, 0);

    yalulSetSubControl(L, YALUL_PROGRESSBAR_LIB);

    lua_pop(L, 1);
    return 0;
}
