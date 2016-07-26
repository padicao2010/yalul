#include "yalul.h"
#include "constant.h"

static int formPadded(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_FORM_LIB));
    int p = uiFormPadded(CAST_ARG(1, Form));
    lua_pushinteger(L, p);
    return 1;
}

static int formSetPadded(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_FORM_LIB));
    uiFormSetPadded(CAST_ARG(1, Form), luaL_checkinteger(L, 2));
    RETURN_SELF;
}

static int formAppend(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_FORM_LIB));

    uiForm *form = CAST_ARG(1, Form);
    const char *name = luaL_checkstring(L, 2);
    uiControl *child = CAST_ARG(3, Control);
    int stretchy = 0;
    if(lua_gettop(L) > 3) {
        stretchy = lua_toboolean(L, 4);
    }

    uiFormAppend(form, name, child, stretchy);
    yalulInsertChild(L, form, child, -1);

    RETURN_SELF;
}

static int formDelete(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_FORM_LIB));

    uiForm *form = CAST_ARG(1, Form);
    int index;
    if(lua_type(L, 2) == LUA_TUSERDATA) {
        uiControl *child = CAST_ARG(2, Control);
        index = yalulGetChildIndex(L, (uiControl *)form, child);
    } else {
        index = luaL_checkinteger(L, 2);
    }
    assert(index > 0);

    uiFormDelete(form, index - 1);
    yalulReleaseChild(L, form, index);

    RETURN_SELF;
}

static struct luaL_Reg meta_Form[] = {
    { "append",                 formAppend },
    { "delete",                 formDelete },
    { "padded",                 formPadded },
    { "setPadded",              formSetPadded },

    { NULL }
};

int yalulSetForm(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Form, 0);

    yalulSetSubControl(L, YALUL_FORM_LIB);

    lua_pop(L, 1);
    return 0;
}
