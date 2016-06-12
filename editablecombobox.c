#include "yalul.h"
#include "constant.h"

static int eComboAppend(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_EDITABLECOMBOBOX_LIB));
    uiEditableCombobox *c = CAST_ARG(1, EditableCombobox);
    for(int i = 2; i <= lua_gettop(L); i++) {
        uiEditableComboboxAppend(c, luaL_checkstring(L, i));
    }
    RETURN_SELF;
}

static int eComboText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_EDITABLECOMBOBOX_LIB));

    char *text = uiEditableComboboxText(CAST_ARG(1, EditableCombobox));
    lua_pushstring(L, text);
    uiFreeText(text);

    return 1;
}

static int eComboSetText(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_EDITABLECOMBOBOX_LIB));
    uiEditableComboboxSetText(CAST_ARG(1, EditableCombobox),
                              luaL_checkstring(L, 2));
    RETURN_SELF;
}

static void callback_onchanged(uiEditableCombobox *c, void *p)
{
    lua_State *L = (lua_State*)p;
    char *text = uiEditableComboboxText(c);
    lua_pushstring(L, text);
    uiFreeText(text);
    yalulDoCallback(L, c, YALUL_ONCHANGED, 1);
}

static int eComboOnChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_EDITABLECOMBOBOX_LIB));
    uiEditableCombobox *c = CAST_ARG(1, EditableCombobox);

    yalulRegisterCallback(L, c, YALUL_ONCHANGED);

    uiEditableComboboxOnChanged(c, callback_onchanged, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_EditableCombobox[] = {
    { "append",                 eComboAppend },
    { "text",                   eComboText },
    { "setText",                eComboSetText },
    { "onChanged",              eComboOnChanged },
    { NULL }
};

int yalulSetEditableCombobox(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_EditableCombobox, 0);

    yalulSetSubControl(L, YALUL_EDITABLECOMBOBOX_LIB);

    lua_pop(L, 1);
    return 0;
}
