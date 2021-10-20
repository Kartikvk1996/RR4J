#pragma once
#include "global_includes.hpp"

#ifndef __NATIVE_FUNCTIONS__HPP
#define __NATIVE_FUNCTIONS__HPP
#include "nativefunctions.hpp"
#endif // __NATIVE_FUNCTIONS_HPP

#ifndef __CONFIG__HPP
#define __CONFIG__HPP
#include "config.hpp"
#endif // __CONFIG__HPP

#ifndef __RECORD_HANDLER__HPP
#define __RECORD_HANDLER__HPP
#include "recordhandler.hpp"
#endif // __RECORD_HANDLER__HPP

extern jvmtiEnv *jvmEnvironment;
extern JavaVM *javaVm;
extern JNIEnv *javaNativeInterface;
extern NativeFunctions nativeFunctions;

class Agent
{
private:
    std::shared_ptr<Configure> configuration;
    std::unordered_map<std::string, std::shared_ptr<RecordHandler>> recordHandlerMap;

public:

    ~Agent()
    {
        configuration.reset();
    }

    static void memAllocate(jlong size, unsigned char *ptr);
    static void memDeAllocate(void *ptr);
    static bool checkErrorAndLog(jvmtiError error);
    static std::string convertJStringToCPPString(JNIEnv *env, jstring value);

    bool init();
    bool start();
    bool createRecordHandler(JNIEnv *env, jstring threadName, jthread thread);
    bool deleteRecordHandler(JNIEnv *env, jstring threadName);
    std::shared_ptr<RecordHandler> getRecordHandler(JNIEnv *env, jstring threadName);
    std::shared_ptr<RecordHandler> getRecordHandler(JNIEnv *env, std::string threadName);

    std::shared_ptr<Configure> getCongiguration();
    std::unordered_map<std::string, std::shared_ptr<RecordHandler>>& getRecordHandlerMap();

};

// Initialized by JVM VMInit callback -> check callbacks.hpp
extern Agent *gblAgent;
