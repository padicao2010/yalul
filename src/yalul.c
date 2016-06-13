#include "yalul.h"
#include "constant.h"

void stackDump (lua_State *L) {
     int i;
     int top = lua_gettop(L);

     for (i = 1; i <= top; i++) {  /* repeat for each level */
       int t = lua_type(L, i);
       switch (t) {
         case LUA_TSTRING:  /* strings */
           printf("`%s'", lua_tostring(L, i));
           break;
         case LUA_TBOOLEAN:  /* booleans */
           printf(lua_toboolean(L, i) ? "true" : "false");
           break;
         case LUA_TNUMBER:  /* numbers */
           printf("%g", lua_tonumber(L, i));
           break;
         default:  /* other values */
           printf("%s", lua_typename(L, t));
           break;
       }
       printf("  ");  /* put a separator */
     }
     printf("\n");  /* end the listing */
}

static void unsetRegistry(lua_State *L, void *ctr)
{
    lua_pushlightuserdata(L, ctr);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);
}

static void gcAllRegistry(lua_State *L)
{
    uiControl *ctr = (uiControl *)lua_topointer(L, -1);
    //printf("GC %p\n", ctr);

    lua_gettable(L, LUA_REGISTRYINDEX);
    if(lua_isnil(L, -1)) {
        lua_pop(L, 1);
    } else {
        for(int i = 1; ; i++) {
            lua_geti(L, -1, i);
            if(lua_isnil(L, -1)) {
                lua_pop(L, 1);
                break;
            }
            gcAllRegistry(L);
        }
        lua_pop(L, 1);
        unsetRegistry(L, ctr);
    }
}

static int yalulGC(lua_State *L)
{
    int checked = yalulCheckControl(L, -1, NULL);
    uiControl *ctr = CAST_ARG(1, Control);

    if(!checked || uiControlParent(ctr) == NULL) {
        //printf("destroyed it: %p\n", ctr);
        if(checked) {
            //printf("REAL destroy %p\n", ctr);
            uiControlDestroy(ctr);
        }

        lua_pushlightuserdata(L, ctr);
        gcAllRegistry(L);
    } else {
        //printf("Has parent, set it: %p\n", ctr);
        lua_pushlightuserdata(L, ctr);
        lua_gettable(L, LUA_REGISTRYINDEX);
        lua_pushboolean(L, 1);
        lua_setfield(L, -2, "isGCed");
        lua_pop(L, 1);
    }
    return 0;
}

static int yalulCreateObject(lua_State *L, void *object, const char *clsname)
{
    void **pp = (void **)lua_newuserdata(L, sizeof(void *));
    *pp = object;

    luaL_setmetatable(L, clsname);

    lua_newtable(L);
    lua_pushboolean(L, 0);
    lua_setfield(L, -2, "isGCed");
    lua_pushlightuserdata(L, object);
    lua_pushvalue(L, -2);
    lua_settable(L, LUA_REGISTRYINDEX);
    lua_pop(L, 1);

    return 0;
}

int yalulDoCallback(lua_State *L, void *control, const char *name, int nargs)
{
    lua_pushlightuserdata(L, control);
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_getfield(L, -1, name);

    if(lua_isnil(L, -1)) {
        lua_pop(L, 2 + nargs);
        return 0;
    }

    if(nargs > 0)
        lua_rotate(L, -2 - nargs, -nargs);
    lua_call(L, nargs, 1);
    int ret = lua_toboolean(L, -1);
    lua_pop(L, 2);

    return ret;
}

int yalulRegisterCallback(lua_State *L, void *control, const char *name)
{
    lua_pushlightuserdata(L, control);
    lua_gettable(L, LUA_REGISTRYINDEX);

    lua_pushvalue(L, -2);
    lua_setfield(L, -2, name);
    lua_pop(L, 1);
}

int yalulSetChild(lua_State *L, void *control, void *child, int index)
{
    //printf("SET %p to %p\n", child, control);
    lua_pushlightuserdata(L, control);
    lua_gettable(L, LUA_REGISTRYINDEX);

    lua_pushlightuserdata(L, child);
    lua_seti(L, -2, index);
    lua_pop(L, 1);
}

int yalulInsertChild(lua_State *L, void *control, void *child, int index)
{
    lua_pushlightuserdata(L, control);
    lua_gettable(L, LUA_REGISTRYINDEX);

    lua_len(L, -1);
    int length = lua_tointeger(L, -1);
    lua_pop(L, 1);

    if(index <= 0) {
        index = length + 1;
    }

    for(int i = length; i >= index; i--) {
        lua_geti(L, -1, i);
        lua_seti(L, -2, i + 1);
    }

    lua_pushlightuserdata(L, child);
    lua_seti(L, -2, index);
    lua_pop(L, 1);
}

int yalulReleaseChild(lua_State *L, void *control, int index)
{
    lua_pushlightuserdata(L, control);
    lua_gettable(L, LUA_REGISTRYINDEX);

    lua_geti(L, -1, index);
    if(lua_isnil(L, -1)) {
        lua_pop(L, 2);
        return 0;
    }

    uiControl *child = (uiControl *)lua_topointer(L, -1);
    lua_gettable(L, LUA_REGISTRYINDEX);
    lua_getfield(L, -1, "isGCed");
    if(lua_toboolean(L, -1)) {
        //printf("destroyed it:%p\n", child);
        uiControlDestroy(child);

        lua_pushlightuserdata(L, child);
        gcAllRegistry(L);
    }
    lua_pop(L, 2);

    for(int i = index; ; i++) {
        lua_geti(L, -1, i + 1);
        if(lua_isnil(L, -1)) {
            lua_seti(L, -2, i);
            break;
        }
        lua_seti(L, -2, i);
    }

    lua_pop(L, 1);
}

int yalulGetChildIndex(lua_State *L, uiControl *parent, uiControl *child)
{
    lua_pushlightuserdata(L, parent);
    lua_gettable(L, LUA_REGISTRYINDEX);

    for(int i = 1; ; i++) {
        lua_geti(L, -1, i);
        if(lua_isnil(L, -1)) {
            lua_pop(L, 2);
            return -1;
        }
        if(lua_topointer(L, -1) == child) {
            lua_pop(L, 2);
            return i;
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 11);
    return -1;
}

int yalulCheckControl(lua_State *L, int index, const char *name)
{
    lua_pushstring(L, YALUL_INITED);
    lua_gettable(L, LUA_REGISTRYINDEX);
    if(!lua_toboolean(L, -1)) {
        lua_pop(L, 1);
        return 0;
    }
    lua_pop(L, 1);

    if(name != NULL && luaL_checkudata(L, index, name) == NULL) {
        return 0;
    }
    return 1;
}

int yalulSetSubControl(lua_State *L, const char *name)
{
    lua_newtable(L);                        // Set Control as subclass of SubControl
    lua_pushvalue(L, -3);
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);

    luaL_newmetatable(L, name);             // Register SubControl
    lua_pushvalue(L, -2);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, yalulGC);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    return 0;
}

static int yalulInit(lua_State *L)
{
    uiInitOptions o;

    memset(&o, 0, sizeof (uiInitOptions));

    const char *err = uiInit(&o);

    lua_pushstring(L, YALUL_INITED);
    lua_pushboolean(L, 1);
    lua_settable(L, LUA_REGISTRYINDEX);

    lua_pushstring(L, err);

    return 1;
}

static int yalulUninit(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    lua_pushstring(L, YALUL_INITED);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    uiUninit();
    return 0;
}

static int yalulMain(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiMain();
    return 0;
}

static int yalulQuit(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));

    lua_pushstring(L, YALUL_INITED);
    lua_pushnil(L);
    lua_settable(L, LUA_REGISTRYINDEX);

    uiQuit();
    return 0;
}

static int yalulOpenFile(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));

    char *file = uiOpenFile(CAST_ARG(1, Window));
    if(file == NULL) {
        lua_pushnil(L);
    } else {
        lua_pushstring(L, file);
        uiFreeText(file);
    }

    return 1;
}

static int yalulSaveFile(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));

    char *file = uiSaveFile(CAST_ARG(1, Window));
    if(file == NULL) {
        lua_pushnil(L);
    } else {
        lua_pushstring(L, file);
        uiFreeText(file);
    }

    return 1;
}

static int yalulMsgBox(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));

    uiMsgBox(CAST_ARG(1, Window),
             luaL_checkstring(L, 2),
             luaL_checkstring(L, 3));

    return 0;
}

static int yalulMsgBoxError(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_WINDOW_LIB));

    uiMsgBoxError(CAST_ARG(1, Window),
             luaL_checkstring(L, 2),
             luaL_checkstring(L, 3));

    return 0;
}

static int yalulNewWindow(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiWindow *win = uiNewWindow(
        luaL_checkstring(L, 1),
        luaL_checkinteger(L, 2),
        luaL_checkinteger(L, 3),
        lua_toboolean(L, 4)
    );

    yalulCreateObject(L, win, YALUL_WINDOW_LIB);

    return 1;
}

static int yalulNewCheckbox(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiCheckbox *box = uiNewCheckbox(luaL_checkstring(L, 1));

    yalulCreateObject(L, box, YALUL_CHECKBOX_LIB);

    return 1;
}

static int yalulNewLabel(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiLabel *label = uiNewLabel(luaL_checkstring(L, 1));

    yalulCreateObject(L, label, YALUL_LABEL_LIB);

    return 1;
}

static int yalulNewButton(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiButton *button = uiNewButton(luaL_checkstring(L, 1));

    yalulCreateObject(L, button, YALUL_BUTTON_LIB);

    return 1;
}

static int yalulNewHBox(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiBox *box = uiNewHorizontalBox();
    yalulCreateObject(L, box, YALUL_BOX_LIB);
    return 1;
}

static int yalulNewVBox(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiBox *box = uiNewVerticalBox();
    yalulCreateObject(L, box, YALUL_BOX_LIB);
    return 1;
}

static int yalulNewEntry(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiEntry *entry = uiNewEntry();
    yalulCreateObject(L, entry, YALUL_ENTRY_LIB);
    return 1;
}

/*
static int yalulNewPasswordEntry(lua_State *L)
{
    uiEntry *entry = uiNewPasswordEntry();
    yalulCreateObject(L, entry, YALUL_ENTRY_LIB);
    return 1;
}
*/

static int yalulNewProgressBar(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiProgressBar *bar = uiNewProgressBar();
    yalulCreateObject(L, bar, YALUL_PROGRESSBAR_LIB);
    return 1;
}

static int yalulNewRadioButtons(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiRadioButtons *r = uiNewRadioButtons();
    yalulCreateObject(L, r, YALUL_RADIOBUTTONS_LIB);
    return 1;
}

static int yalulNewColorButton(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiColorButton *r = uiNewColorButton();
    yalulCreateObject(L, r, YALUL_COLORBUTTON_LIB);
    return 1;
}

static int yalulNewCombobox(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiCombobox *c = uiNewCombobox();
    yalulCreateObject(L, c, YALUL_COMBOBOX_LIB);
    return 1;
}

static int yalulNewEditableCombobox(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiEditableCombobox *c = uiNewEditableCombobox();
    yalulCreateObject(L, c, YALUL_EDITABLECOMBOBOX_LIB);
    return 1;
}

static int yalulNewSlider(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiSlider *c = uiNewSlider(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    yalulCreateObject(L, c, YALUL_SLIDER_LIB);
    return 1;
}

static int yalulNewSpinbox(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiSpinbox *sb = uiNewSpinbox(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    yalulCreateObject(L, sb, YALUL_SPINBOX_LIB);
    return 1;
}

static int yalulNewGroup(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiGroup *group = uiNewGroup(luaL_checkstring(L, 1));
    yalulCreateObject(L, group, YALUL_GROUP_LIB);
    return 1;
}

static int yalulNewTab(lua_State *L)
{
    assert(yalulCheckControl(L, 0, NULL));
    uiTab *tab = uiNewTab();
    yalulCreateObject(L, tab, YALUL_TAB_LIB);
    return 1;
}


static struct luaL_Reg yalul_table[] = {
    { "init",                   yalulInit },
    { "uninit",                 yalulUninit },
    { "main",                   yalulMain },
    { "quit",                   yalulQuit },

    { "newWindow",              yalulNewWindow },
    { "newCheckbox",            yalulNewCheckbox },
    { "newLabel",               yalulNewLabel},
    { "newButton",              yalulNewButton},
    { "newHBox",                yalulNewHBox},
    { "newVBox",                yalulNewVBox},
    { "newEntry",               yalulNewEntry },
//    { "newPasswordEntry",       yalulNewPasswordEntry },
    { "newProgressBar",         yalulNewProgressBar },
    { "newRadioButtons",        yalulNewRadioButtons },
    { "newColorButton",         yalulNewColorButton },
    { "newCombobox",            yalulNewCombobox },
    { "newEditableCombobox",    yalulNewEditableCombobox },
    { "newSlider",              yalulNewSlider },
    { "newSpinbox",             yalulNewSpinbox },
    { "newGroup",               yalulNewGroup },
    { "newTab",                 yalulNewTab },

    { "openFile",               yalulOpenFile },
    { "saveFile",               yalulSaveFile },
    { "msgBox",                 yalulMsgBox },
    { "msgBoxError",            yalulMsgBoxError },

    { NULL }
};

int luaopen_libyalul(lua_State *L)
{
    luaL_newlib(L, yalul_table);    // libyalul = { ... }

    yalulSetControl(L);

    yalulSetWindow(L);
    yalulSetCheckbox(L);
    yalulSetLabel(L);
    yalulSetButton(L);
    yalulSetBox(L);
    yalulSetEntry(L);
    yalulSetProgressBar(L);
    yalulSetRadioButtons(L);
    yalulSetColorButton(L);
    yalulSetCombobox(L);
    yalulSetEditableCombobox(L);
    yalulSetSlider(L);
    yalulSetSpinbox(L);
    yalulSetGroup(L);
    yalulSetTab(L);

    lua_pop(L, 1);

    return 1;
}
