#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

bool Agent::init()
{
    configuration = std::make_shared<Configure>();
    configuration->init();
    if(configuration->registerNativeMethods())
    {
        PLOGI<<"Successfully initialized agent";
        return true;
    }
    PLOGI<<"Failed to initialize the agent";
    return false;
}

// Nothing to start
bool Agent::start()
{
    return true;
}

bool Agent::createRecordHandler(JNIEnv *env, jstring threadName, jthread thread)
{
    std::string tName = Agent::convertJStringToCPPString(env, threadName);
    if(recordHandlerMap.find(tName) == recordHandlerMap.end())
    {
        PLOGI<<"Creating new Record Handler for thread '"<<tName<<"'";
        std::shared_ptr<RecordHandler> rhandler = std::make_shared<RecordHandler>(tName, thread);
        recordHandlerMap.insert(std::make_pair(tName, rhandler));
        PLOGI<<"Successfully created new Record Handler for thread '"<<tName<<"', tId : "<<thread;
        return true;
    }
    else
    {
        PLOGI<<"Record Handler for thread '"<<threadName<<"' is already present";
        return false;
    }
}

bool Agent::deleteRecordHandler(JNIEnv *env, jstring threadName)
{
    std::string tName = Agent::convertJStringToCPPString(env, threadName);
    if(recordHandlerMap.find(tName) != recordHandlerMap.end())
    {
        recordHandlerMap[tName].reset();
        recordHandlerMap.erase(tName);
        PLOGI<<"De-registered thread '"<<tName<<"'";
        return true;
    }
    PLOGW<<"Failed to de-register thread '"<<tName<<"', thread was not registered earlier";
    return false;
}

void Agent::memAllocate(jlong size, unsigned char* ptr)
{
    ptr = NULL;
    jvmtiError error = jvmEnvironment->Allocate(size, &ptr);
    checkErrorAndLog(error);
}

void Agent::memDeAllocate(void *ptr)
{
    jvmtiError error = jvmEnvironment->Deallocate((unsigned char *)ptr);
    checkErrorAndLog(error);
}

std::string Agent::convertJStringToCPPString(JNIEnv *env, jstring value)
{
    const char *mName = env->GetStringUTFChars(value, 0);
    std::string temp = mName;
    env->ReleaseStringUTFChars(value, mName);
    return temp;
}

bool Agent::checkErrorAndLog(jvmtiError error)
{
    if(error != JVMTI_ERROR_NONE)
    {
        char *errorName = NULL;
        jvmEnvironment->GetErrorName(error, &errorName);
        if(errorName != NULL)
        {
            PLOGE<< "JVM error, errorCode : "<<error<< " errorName : "<< errorName;
            jvmEnvironment->Deallocate((unsigned char *)errorName);
        }
        return true;
    }
    return false;
}

std::shared_ptr<Configure> Agent::getCongiguration()
{
    return configuration;
}

std::unordered_map<std::string, std::shared_ptr<RecordHandler>>& Agent::getRecordHandlerMap()
{
    return recordHandlerMap;
}

std::shared_ptr<RecordHandler> Agent::getRecordHandler(JNIEnv *env, jstring threadName)
{
    std::string tName = Agent::convertJStringToCPPString(env, threadName);
    if(recordHandlerMap.find(tName) != recordHandlerMap.end())
        return recordHandlerMap.at(tName);
    else
    {
        return nullptr;
    }
}

std::shared_ptr<RecordHandler> Agent::getRecordHandler(JNIEnv *env, std::string threadName)
{
    if(recordHandlerMap.find(threadName) != recordHandlerMap.end())
        return recordHandlerMap.at(threadName);
    else
    {
        return nullptr;
    }
}

// TODO handle VM death caused by application....
