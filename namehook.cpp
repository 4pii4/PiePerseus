
int loggedIn(lua_State *L) {
    // getProxy(UserProxy):GetLoginedFlag
    lua_getglobal(L, STR("getProxy"));
    lua_getglobal(L, STR("UserProxy"));
    lua_pcall(L, 1, 1, 0);

    lua_getfield(L, -1, STR("GetLoginedFlag"));
    lua_insert(L, -2);
    lua_pcall(L, 1, 1, 0);
    
    return lua_tonumber(L, -1);
}

// this gets 2 arguments, a `this` pointer and smth else
int hookPlayerCtor(lua_State *L) {
    // calls original ctor first
    lua_getfield(L, 1, STR("oldNew"));
    lua_pushvalue(L, 1);
    lua_pushvalue(L, 2);
    lua_pcall(L, 2, 0, 0);

    // modifies name only if isSelf
    lua_getfield(L, 1, STR("isSelf"));
    lua_pushvalue(L, 1);
    lua_pcall(L, 1, 1, 0);
    int isSelf = (int) lua_tonumber(L, -1);

    if (isSelf && loggedIn(L)) {
        lua_insert(L, 1);
        replaceAttributeN(L, STR("name"), STR("adolf the man"));
    }
    
    lua_pop(L, 2);
    return 0;
}

void nameHook(lua_State *L) {
    luaHookFunc(L, OBFUSCATE("Player.New"), hookPlayerCtor, "old");
}
