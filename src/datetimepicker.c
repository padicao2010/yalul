#include "yalul.h"
#include "constant.h"

static struct luaL_Reg meta_DateTimePicker[] = {
    { NULL }
};

int yalulSetDateTimePicker(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_DateTimePicker, 0);

    yalulSetSubControl(L, YALUL_DATETIMEPICKER_LIB);

    lua_pop(L, 1);
    return 0;
}
