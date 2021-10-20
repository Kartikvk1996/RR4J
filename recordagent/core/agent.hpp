#pragma once
#include "global_includes.hpp"

#ifndef __CONFIG__HPP
#define __CONFIG__HPP
#include "config.hpp"
#endif // __CONFIG__HPP

extern jvmtiEnv *jvmEnvironment;
extern JavaVM *javaVm;
extern JNIEnv *javaNativeInterface;
extern std::unordered_map<std::string, jclass> gblClassMap;

class Agent
{
private:
    std::shared_ptr<Configure> configuration;

public:

    bool init();
    bool start();

    ~Agent()
    {
        configuration.reset();
        gblClassMap.clear();
    }

    static void memAllocate(jlong size, unsigned char *ptr);
    static void memDeAllocate(void *ptr);
    static bool checkErrorAndLog(jvmtiError error);
    void startOnThread();
    std::shared_ptr<Configure> getCongiguration();
};

// Initialized by JVM VMInit callback -> check callbacks.hpp
extern Agent *gblAgent;
