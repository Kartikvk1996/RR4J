#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

#ifndef __EXCEPTION__HPP
#define __EXCEPTION__HPP
#include "exception.hpp"
#endif // __EXCEPTION__HPP

bool Exception::init()
{
    klass = javaNativeInterface->FindClass("com/rr4j/record/JVMException");
    if(klass == NULL)
    {
        PLOGE<<"Failed to find class 'com.rr4j.record.JVMException'";
        return false;
    }
    return true;
}

bool Exception::exceptionCallback(JNIEnv* jni_env, jthread thread, jobject exceptn, jstring className, jstring methodName)
{
    exceptionCallbackMtdId = jni_env->GetStaticMethodID(klass, "exceptionCallback", "(Ljava/lang/Thread;Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;)Z");
    if(exceptionCallbackMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.JVMException::exceptionCallback'";
        return false;
    }

    bool res = jni_env->CallStaticBooleanMethod(klass, exceptionCallbackMtdId, thread, exceptn, className, methodName);
    if(res == true)
    {
        jvmtiThreadInfo threadInfo;
        jvmtiError error = jvmEnvironment->GetThreadInfo(thread, &threadInfo);
        if(!Agent::checkErrorAndLog(error))
        {
            PLOGI<<"Exception raised by thread : '"<<threadInfo.name<<"' was handled and it matched the rule exception";
            Agent::memDeAllocate(threadInfo.name);
        }
        else
        {
            PLOGI<<"Exception raised by thread : '"<<thread<<"' [failed to get thread name] was handled and it matched the rule exception";
        }
        return true;
    }
    return false;
}
