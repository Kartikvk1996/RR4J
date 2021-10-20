#ifndef __CONFIG__HPP
#define __CONFIG__HPP
#include "config.hpp"
#endif // __CONFIG__HPP

#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

void Configure::init()
{
    plog::init(plog::debug, logFileName.c_str(), logFileMaxSize, maxLogFiles);
    PLOGI << "Initialized the logger. FileName = "<<logFileName<<" MaxSize = "<<logFileMaxSize<<" MaxLogFiles = "<<maxLogFiles;
}

bool Configure::registerNativeMethods()
{
    recordExecutor = javaNativeInterface->FindClass("com/recordreplay/replay/handles/RecordHandler");
    if(recordExecutor == NULL)
    {
        PLOGE<<"Failed to find class 'com.recordreplay.replay.handles.RecordHandler'";
        return false;
    }

    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerThread", std::make_pair("(Ljava/lang/String;Ljava/lang/Thread;)V", (void *)nativeFunctions.NativeCall_registerThread)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_deregisterThread", std::make_pair("(Ljava/lang/String;Ljava/lang/Thread;)V", (void *)nativeFunctions.NativeCall_deregisterThread)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_startMonitoring", std::make_pair("(Ljava/lang/String;Ljava/lang/Thread;)V", (void *)nativeFunctions.NativeCall_startMonitoring)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_stopMonitoring", std::make_pair("(Ljava/lang/String;Ljava/lang/Thread;)V", (void *)nativeFunctions.NativeCall_stopMonitoring)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRBoolean", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZI)V", (void *)nativeFunctions.NativeCall_registerMethodRBoolean)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRByte", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;BI)V", (void *)nativeFunctions.NativeCall_registerMethodRByte)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRChar", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;CI)V", (void *)nativeFunctions.NativeCall_registerMethodRChar)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRShort", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;SI)V", (void *)nativeFunctions.NativeCall_registerMethodRShort)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRInt", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;II)V", (void *)nativeFunctions.NativeCall_registerMethodRInt)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRLong", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JI)V", (void *)nativeFunctions.NativeCall_registerMethodRLong)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRFloat", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;FI)V", (void *)nativeFunctions.NativeCall_registerMethodRFloat)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRDouble", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;DI)V", (void *)nativeFunctions.NativeCall_registerMethodRDouble)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRVoid", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V", (void *)nativeFunctions.NativeCall_registerMethodRVoid)));
    nativeFunctionsMap.insert(std::make_pair("NativeCall_registerMethodRObject", std::make_pair("(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;I)V", (void *)nativeFunctions.NativeCall_registerMethodRObject)));

    JNINativeMethod nMethods[nativeFunctionsMap.size()];

    int cntr = 0;
    for(auto ittr = nativeFunctionsMap.begin(); ittr != nativeFunctionsMap.end(); ittr++)
    {
        PLOGI<<"Registering method '"<<ittr->first<<"'";
        nMethods[cntr].name = (char *)ittr->first.c_str();
        nMethods[cntr].signature = (char *)(ittr->second).first.c_str();
        nMethods[cntr].fnPtr = (void *)(ittr->second.second);
        cntr++;
    }

    int status = javaNativeInterface->RegisterNatives(recordExecutor, nMethods, nativeFunctionsMap.size());
    if(status != 0)
    {
        PLOGE<<"Failed to register native methods for class 'com.recordreplay.replay.handles.RecordHandler'";
        return false;
    }
    PLOGI<<"Successfully registered native methods for class 'com.recordreplay.replay.handles.RecordHandler'";
    return true;
}
