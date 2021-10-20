// JVMTI Callbacks

#ifndef __RECORD_DATA__HPP
#define __RECORD_DATA__HPP
#include "recorddata.hpp"
#endif // __RECORD_DATA__HPP

static void JNICALL callbackVMInit(jvmtiEnv *jvmti, JNIEnv *env, jthread thread)
{
    javaNativeInterface = env;
    gblAgent = new Agent();
    if(gblAgent->init()==false)
    {
        std::cout<< "Failed to initialize agent" << "\n";
        return;
    }

    if(gblAgent->start()==false)
    {
        std::cout<< "Failed to start agent" << "\n";
        return;
    }
}

static void JNICALL callbackMethodEntry(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method)
{
    jvmtiThreadInfo info;
    jvmtiError error = jvmti_env->GetThreadInfo(thread, &info);
    if(!Agent::checkErrorAndLog(error))
    {
        std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(jni_env, std::string(info.name));
        if(handler != nullptr)
        {
            if(handler->canMethodBeForcedToReturn(method) == true)
            {
                RecordMethodData data = handler->getMethodData(method);
                switch(data.methodType)
                {
                    error = JVMTI_ERROR_NOT_AVAILABLE;
                    case 'B' :  error = jvmti_env->ForceEarlyReturnInt(thread, data.rValue.b); break;
                    case 'C' :  error = jvmti_env->ForceEarlyReturnInt(thread, data.rValue.c); break;
                    case 'S' :  error = jvmti_env->ForceEarlyReturnInt(thread, data.rValue.s); break;
                    case 'Z' :  error = jvmti_env->ForceEarlyReturnInt(thread, data.rValue.z); break;
                    case 'I' :  error = jvmti_env->ForceEarlyReturnInt(thread, data.rValue.i); break;
                    case 'J' :  error = jvmti_env->ForceEarlyReturnLong(thread, data.rValue.j); break;
                    case 'F' :  error = jvmti_env->ForceEarlyReturnFloat(thread, data.rValue.f); break;
                    case 'D' :  error = jvmti_env->ForceEarlyReturnDouble(thread, data.rValue.d); break;
                    case 'L' :
                        {
                            error = jvmti_env->ForceEarlyReturnObject(thread, data.rValue.l);
                            jni_env->DeleteGlobalRef(data.rValue.l);
                            break;
                        }

                    case 'V' :  error = jvmti_env->ForceEarlyReturnVoid(thread); break;
                    default: PLOGE<<"Unmatched return type cannot force thread return, Method name : '"<<data.methodName<<"', Method type : '"<<data.methodType<<"'"; break;

                }
                if(!Agent::checkErrorAndLog(error))
                    PLOGI<<"Returned recorded value for methodId '"<<method<<"'";
            }
        }
        Agent::memDeAllocate(info.name);
    }

    return;
}

static void JNICALL callbackMethodExit(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method, jboolean was_popped_by_exception, jvalue return_value)
{

}

static void NativeCall_startMonitoring(JNIEnv *env, jclass klass, jstring threadName, jthread thread)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
    {
        handler->computeMaxDepth();
        PLOGI<<"Obtained thread '"<<thread<<"' from handler";
        jvmtiError error = jvmEnvironment->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, thread);
        if(Agent::checkErrorAndLog(error) == true)
        {
            PLOGI<<"Failed, Can't watch thread execution";
            return;
        }

        error = jvmEnvironment->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_EXIT, thread);
        if(Agent::checkErrorAndLog(error) == true)
        {
            PLOGI<<"Failed, Can't watch thread execution";
            error = jvmEnvironment->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_ENTRY, thread);
            return;
        }
        PLOGI<<"Watching thread '"<<thread<<"' execution";
    }
    else
    {
        PLOGE<<"Could not find record handler to start thread monitoring, make sure thread is registered";
    }
}

static void NativeCall_stopMonitoring(JNIEnv *env, jclass klass, jstring threadName, jthread thread)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
    {
        PLOGI<<"Obtained thread '"<<thread<<"' from handler";
        jvmtiError error = jvmEnvironment->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_ENTRY, thread);
        Agent::checkErrorAndLog(error);
        error = jvmEnvironment->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_EXIT, thread);
        Agent::checkErrorAndLog(error);
        PLOGI<<"Stopped Watching thread '"<<thread<<"' execution";
    }
    else
    {
        PLOGE<<"Could not find record handler to stop thread monitoring, make sure thread is registered";
    }
}


static void NativeCall_registerThread(JNIEnv *env, jclass klass, jstring threadName, jthread thread)
{
    PLOGI<<"Thread registered "<<thread;
    gblAgent->createRecordHandler(env, threadName, thread);
}

static void NativeCall_deregisterThread(JNIEnv *env, jclass klass, jstring threadName, jthread thread)
{
    PLOGI<<"Thread de-registered "<<thread;
    gblAgent->deleteRecordHandler(env, threadName);
}

static void NativeCall_registerMethodRBoolean(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jboolean returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRBoolean(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRByte(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jbyte returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRByte(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRChar(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jchar returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRChar(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRShort(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jshort returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRShort(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRInt(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jint returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRInt(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRLong(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jlong returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRLong(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRFloat(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jfloat returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRFloat(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRDouble(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jdouble returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRDouble(env, methodName, klassName, returnValue, depth);
}

static void NativeCall_registerMethodRVoid(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRVoid(env, methodName, klassName, depth);
}

static void NativeCall_registerMethodRObject(JNIEnv *env, jclass klass, jstring threadName, jstring methodName, jstring klassName, jobject returnValue, jint depth)
{
    std::shared_ptr<RecordHandler> handler = gblAgent->getRecordHandler(env, threadName);
    if(handler != nullptr)
        handler->registerMethodRObject(env, methodName, klassName, returnValue, depth);
}
