#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include <assert.h>
#include <ui.h>

#define CAST_ARG(n, type) ui ## type(*((void**)lua_touserdata(L, n)))

#define RETURN_SELF lua_pushvalue(L, 1); return 1;

int yalulSetControl(lua_State *L);
int yalulSetWindow(lua_State *L);
int yalulSetCheckbox(lua_State *L);
int yalulSetLabel(lua_State *L);
int yalulSetButton(lua_State *L);
int yalulSetBox(lua_State *L);
int yalulSetEntry(lua_State *L);
int yalulSetProgressBar(lua_State *L);
int yalulSetRadioButtons(lua_State *L);
int yalulSetColorButton(lua_State *L);
int yalulSetCombobox(lua_State *L);
int yalulSetEditableCombobox(lua_State *L);
int yalulSetSlider(lua_State *L);
int yalulSetSpinbox(lua_State *L);
int yalulSetGroup(lua_State *L);
int yalulSetTab(lua_State *L);
int yalulSetMenu(lua_State *L);

int yalulDoCallback(lua_State *L, void *control, const char *name, int nargs);
int yalulRegisterCallback(lua_State *L, void *control, const char *name);
int yalulSetChild(lua_State *L, void *control, void *child, int index);
int yalulInsertChild(lua_State *L, void *control, void *child, int index);
int yalulReleaseChild(lua_State *L, void *control, int index);
int yalulGetChildIndex(lua_State *L, uiControl *parent, uiControl *child);
int yalulCheckControl(lua_State *L, int index, const char *name);
int yalulSetSubControl(lua_State *L, const char *name);
int yalulInitMenu(lua_State *L, const char *name);

void stackDump(lua_State *L);
