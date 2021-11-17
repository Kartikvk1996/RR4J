#pragma once
#include "global_includes.hpp"

extern jrawMonitorID lock;

/**
    This class attach the hook to method specified.
**/
class Method
{
public:
    static bool isBkpActive;
    std::vector<std::pair<jmethodID, jlocation>> hooks;

    static const int MethodIn = 0;
    static const int MethodOut = 1;

    ~Method()
    {
        hooks.clear();
    }

    /**
        Attach the hook, so a callback is raised when the method is called by executing thread.
    **/
    bool addHook(std::string name);

    /**
        Clears the hook attached to method.
    **/
    bool clearHook(jmethodID mtd, jlocation loc);

    void clearAllHooks();
};

