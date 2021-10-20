#pragma once
#include "global_includes.hpp"

class Configure
{
private:
    std::string logFileName = "replaylog.log";
    int logFileMaxSize = 1024*1024*10; // 10 MB
    int maxLogFiles = 5;
    jclass recordExecutor;
    std::unordered_map<std::string, std::pair<std::string, void *>> nativeFunctionsMap;

public:
    Configure()
    {
    }

    ~Configure()
    {
    }

    void init();
    bool registerNativeMethods();
};
