#pragma once

// full credit of everything in this file goes to cmdtaves

#include "../Structs.h"

extern struct Config config;

#define tostr(x) (static_cast<std::string>(x))
#define stdstr(x) (static_cast<std::string>(OBFUSCATE(x)))
#define STR(str) il2cpp_string_new(OBFUSCATE(str))

Il2CppString *stdstr2ilstr(std::string s);
void luaHookFunc(lua_State *L, std::string field, lua_CFunction func, std::string backup_prefix);

int wrapPVDPUpdateInfo(lua_State *L) {
    // call original UpdateInfo
    lua_getglobal(L, STR("PlayerVitaeDetailPage"));
    lua_getfield(L, -1, STR("oldUpdateInfo"));
    lua_remove(L, -2);
    lua_pushvalue(L, 1);
    lua_pcall(L, 1, 0, 0);
    
    // override values
    if (!config.Spoof.name.empty()) {
        lua_getfield(L, 1, STR("nameTxt"));
        lua_pushstring(L, stdstr2ilstr(config.Spoof.name));
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);
    }
    if (!config.Spoof.id.empty()) {
        lua_getfield(L, 1, STR("idTxt"));
        lua_pushstring(L, stdstr2ilstr(config.Spoof.id));
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);
    }
    
    if (config.Spoof.lv > 0) {
        std::string l = stdstr("LV.") + std::to_string(config.Spoof.lvInt);
        lua_getfield(L, 1, STR("levelTxt"));
        lua_pushstring(L, stdstr2ilstr(l));
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);

        std::string expTxt = std::to_string(config.Spoof.spoofXp) + stdstr("/") + std::to_string(config.Spoof.lvMax);
        lua_getfield(L, 1, STR("expTxt"));
        lua_pushstring(L, il2cpp_string_new(expTxt.data()));
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);
    }
    return 0;   
}


int wrapMPIBUpdateExp(lua_State *L) {
    
    // call old MainPlayerInfoBtn.UpdateExp
    lua_getglobal(L, STR("MainPlayerInfoBtn"));
    lua_getfield(L, -1, STR("oldUpdateExp"));
    lua_pushvalue(L, 1);
    lua_pcall(L, 1, 0, 0);
    
    // override values
    if (config.Spoof.lv > 0) {
        // replace arg0.expSlider.value with our own spoofLevelFrac
        lua_getfield(L, 1, STR("expSlider"));
        lua_pushnumber(L, config.Spoof.lvFrac);
        lua_setfield(L, -2, STR("value"));
    }
    
    return 0;
}

int wrapNMPIBUpdateExp(lua_State *L) {
    
    // call old MainPlayerInfoBtn4Mellow.UpdateExp
    lua_getglobal(L, STR("MainPlayerInfoBtn4Mellow"));
    lua_getfield(L, -1, STR("oldUpdateExp"));
    lua_pushvalue(L, 1);
    lua_pcall(L, 1, 0, 0);
    
    
    // override values
    if (config.Spoof.lv > 0) {
        // set level and exp text
        lua_getfield(L, 1, STR("playerLevelTxt"));
        lua_pushnumber(L, config.Spoof.lvInt);
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);
        lua_getfield(L, 1, STR("expTxt"));
        lua_pushnumber(L, config.Spoof.lvInt);
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);
        
        // set xp fraction and text scaling
        lua_getfield(L, 1, STR("expTr"));
        lua_getglobal(L, STR("Vector2"));
        lua_pushnumber(L, 70);
        lua_pushnumber(L, (config.Spoof.lvFrac * 34));
        lua_pcall(L, 2, 1, 0);
        lua_setfield(L, -2, STR("sizeDelta"));
    }
    
    return 0;
}

int wrapBDDRVSetFleetVO(lua_State *L) {
    
    bool needNameSpoof = !config.Spoof.name.empty();
    bool needLevelSpoof = (config.Spoof.lv > 0);
    
    // get ys.Battle
    lua_getglobal(L, STR("ys"));
    lua_getfield(L, -1, STR("Battle"));
    
    // check if this bar is the friendly bar
    lua_getfield(L, -1, STR("BattleConfig"));
    lua_getfield(L, -1, STR("FRIENDLY_CODE"));
    int friendlyCode = lua_tonumber(L, -1);
    lua_pop(L, 2);
    lua_getfield(L, 2, STR("GetIFF"));
    lua_pushvalue(L, 2);
    lua_pcall(L, 1, 1, 0);
    int currentCode = lua_tonumber(L, -1);
    lua_pop(L, 1);
    bool isFriendly = (friendlyCode == currentCode);
    
    // get args for old function
    lua_getfield(L, -1, STR("BattleDuelDamageRateView"));
    lua_getfield(L, -1, STR("oldSetFleetVO"));
    lua_remove(L, -2);
    for (int i = 1; i <= 2; i++) {
        lua_pushvalue(L, i);
    }
    // override values
    lua_newtable(L);
    
    lua_pcall(L, 3, 0, 0);
    
    return 0;
}

int wrapMPIBUpdateLevelAndName(lua_State *L) {
    // calls original UpdateLevelAndName
    lua_getglobal(L, STR("MainPlayerInfoBtn"));
    lua_getfield(L, -1, STR("oldUpdateLevelAndName"));
    lua_pushvalue(L, 1);
    lua_pcall(L, 1, 0, 0);
    
    // override values
    if (!config.Spoof.name.empty()) {
        lua_getfield(L, 1, STR("playerNameTxt"));
        lua_pushstring(L, stdstr2ilstr(config.Spoof.name));
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);
    }
    
    if (config.Spoof.lv > 0) {
        std::string l = stdstr("LV.") + std::to_string(config.Spoof.lvInt);
        lua_getfield(L, 1, STR("playerLevelTxt"));
        lua_pushstring(L, stdstr2ilstr(l));
        lua_setfield(L, -2, STR("text"));
        lua_pop(L, 1);
    }
    
    return 0;
}

int wrapPRUIStaticFlush(lua_State *L) {

    // call original StaticFlush
    lua_getglobal(L, STR("PlayerResUI"));
    lua_getfield(L, -1, STR("oldStaticFlush"));
    for (int i = 1; i <= 6; i++) {
        lua_pushvalue(L, i);
    }
    lua_pcall(L, 6, 0, 0);
    
    if (config.Spoof.lv > 0) {
        std::string CoinMaxTxt = "MAX: " + std::to_string(config.Spoof.maxCoins);
        lua_pushstring(L, il2cpp_string_new(CoinMaxTxt.data()));
        lua_setfield(L, 2, STR("text"));
        
        std::string OilMaxTxt = "MAX: " + std::to_string(config.Spoof.maxOil);
        lua_pushstring(L, il2cpp_string_new(OilMaxTxt.data()));
        lua_setfield(L, 4, STR("text"));
    }
    
    return 0;
}

void modSpoof(lua_State *L) {
    luaHookFunc(L, OBFUSCATE("PlayerVitaeDetailPage.UpdateInfo"), wrapPVDPUpdateInfo, OBFUSCATE("old"));
    luaHookFunc(L, OBFUSCATE("MainPlayerInfoBtn.UpdateLevelAndName"), wrapMPIBUpdateLevelAndName, OBFUSCATE("old"));
    luaHookFunc(L, OBFUSCATE("MainPlayerInfoBtn.UpdateExp"), wrapMPIBUpdateExp, OBFUSCATE("old"));
    luaHookFunc(L, OBFUSCATE("MainPlayerInfoBtn4Mellow.UpdateExp"), wrapNMPIBUpdateExp, OBFUSCATE("old"));
    luaHookFunc(L, OBFUSCATE("ys.Battle.BattleDuelDamageRateView.SetFleetVO"), wrapBDDRVSetFleetVO, OBFUSCATE("old"));
    luaHookFunc(L, OBFUSCATE("PlayerResUI.StaticFlush"), wrapPRUIStaticFlush, OBFUSCATE("old"));
}