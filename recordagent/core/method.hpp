#pragma once
#include "global_includes.hpp"

extern jrawMonitorID lock;

class Method{


public:
    static bool isBkpActive;
    std::vector<std::pair<jmethodID, jlocation>> hooks;

    static const int MethodIn = 0;
    static const int MethodOut = 1;

    ~Method()
    {
        hooks.clear();
    }

    bool addHook(std::string name);
    bool clearHook(jmethodID mtd, jlocation loc);
    void clearAllHooks();
};

