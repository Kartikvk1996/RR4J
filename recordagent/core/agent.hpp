#pragma once
#include "global_includes.hpp"

#ifndef __CONFIG__HPP
#define __CONFIG__HPP
#include "config.hpp"
#endif // __CONFIG__HPP

/**
    Global variables which can be accessed.
**/
extern jvmtiEnv *jvmEnvironment;
extern JavaVM *javaVm;
extern JNIEnv *javaNativeInterface;
extern std::unordered_map<std::string, jclass> gblClassMap;


/**
    This is an entry point where we configure the tool.
    Once it is configured, we create a new Java thread and start the
    rule execution on that thread, so even if there is any fatal error
    the thread dies and will not terminate the entire JVM.
**/
class Agent
{
private:
    std::shared_ptr<Configure> configuration;

public:

    /**
        Initializes the Agent
    **/
    bool init();

    /**
        Starts the agent.
    **/
    bool start();

    ~Agent()
    {
        configuration.reset();
        gblClassMap.clear();
    }

    /**
        Static function to allocated requested memory on JVM
    **/
    static void memAllocate(jlong size, unsigned char *ptr);

    /**
        Static function to deallocate the allocated memory by JVM for all the native operations performed.
    **/
    static void memDeAllocate(void *ptr);

    /**
        Checks the error code for each JVM operations and logs if required.
    **/
    static bool checkErrorAndLog(jvmtiError error);

    /**
        Returns the agent configuration
    **/
    std::shared_ptr<Configure> getCongiguration();

public:

    /**
        Starts the agent rules on new thread
    **/
    void startOnThread();
};

// Initialized by JVM VMInit callback -> check callbacks.hpp
extern Agent *gblAgent;
