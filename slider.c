#include "yalul.h"
#include "constant.h"

static int sliderValue(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_SLIDER_LIB));
    int v = uiSliderValue(CAST_ARG(1, Slider));
    lua_pushinteger(L, v);
    return 1;
}

static int sliderSetValue(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_SLIDER_LIB));
    uiSliderSetValue(CAST_ARG(1, Slider), luaL_checkinteger(L, 2));
    RETURN_SELF;
}

static void callback_onchanged(uiSlider *slider, void *p)
{
    lua_State *L = (lua_State*)p;
    int v = uiSliderValue(slider);
    lua_pushinteger(L, v);
    yalulDoCallback(L, slider, YALUL_ONCHANGED, 1);
}

static int sliderOnChanged(lua_State *L)
{
    assert(yalulCheckControl(L, 1, YALUL_SLIDER_LIB));
    uiSlider *slider = CAST_ARG(1, Slider);

    yalulRegisterCallback(L, slider, YALUL_ONCHANGED);

    uiSliderOnChanged(slider, callback_onchanged, L);

    RETURN_SELF;
}

static struct luaL_Reg meta_Slider[] = {
    { "value",                 sliderValue },
    { "setValue",              sliderSetValue },
    { "onChanged",             sliderOnChanged },
    { NULL }
};

int yalulSetSlider(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, meta_Slider, 0);

    yalulSetSubControl(L, YALUL_SLIDER_LIB);

    lua_pop(L, 1);
    return 0;
}
