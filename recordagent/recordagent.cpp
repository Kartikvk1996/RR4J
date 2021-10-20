/**
Main JVM agent loaded at runtime.
**/

// Import all headers files.
#include "core/global_includes.hpp"

#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "core/agent.hpp"
#endif // __AGENT_HPP

#ifndef __RECORD_DIRECTIVE__HPP
#define __RECORD_DIRECTIVE__HPP
#include "core/recorddirective.hpp"
#endif // __RECORD_DIRECTIVE__HPP

#ifndef __EXCEPTION__HPP
#define __EXCEPTION__HPP
#include "core/exception.hpp"
#endif // __EXCEPTION__HPP

#ifndef __SERIALIZER__HPP
#define __SERIALIZER__HPP
#include "core/serializer.hpp"
#endif // __SERIALIZER__HPP


jvmtiEnv *jvmEnvironment;
JavaVM *javaVm;
JNIEnv *javaNativeInterface;
jvmtiEventCallbacks callbacks;
Agent *gblAgent;
RecordDirective *gblRecordDirective;
Exception *gblException;
Serializer *gblSerializer;

jrawMonitorID gblExceptionCallbackLock;

std::unordered_map<std::string, jclass> gblClassMap;

std::mutex threadsMonitoredCountLock;
std::mutex recordCompleteLock;
std::mutex tracingCompleteLock;
std::mutex transformationCompleteLock;



#include "core/callbacks.hpp"

/**

Called by JVM at runtime when the agent is loaded.
This method initializes the jvmti environment and enables all the
capabilities.

**/
JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM* vm, char *options, void *reserved)
{
    jvmtiEnv *jEnv = NULL;
    JNIEnv *jni = NULL;
    jint opResult;

    opResult = vm->GetEnv((void **) &jEnv, JVMTI_VERSION_1_2);

    if(opResult != JNI_OK || jEnv == NULL)
    {
        std::cout << "Failed to get JVM environment, Error code : "<< opResult;
        return JNI_OK;
    }

    opResult = vm->GetEnv((void **) &jni, JVMTI_VERSION_1_2);

    if(opResult != JNI_OK || jEnv == NULL)
    {
        std::cout << "Failed to get Java Native Interface, Error code : "<< opResult;
        return JNI_OK;
    }

    std::cout<<"Live mode\n";
    jvmtiCapabilities capabilities;
    memset(&capabilities, 0, sizeof(capabilities));
    jEnv->GetCapabilities(&capabilities);

    // enable event generation.
    capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_generate_method_exit_events = 1;
    capabilities.can_generate_method_entry_events = 1;
    capabilities.can_generate_exception_events = 1;
    capabilities.can_get_line_numbers = 1;
    capabilities.can_get_source_file_name = 1;
    capabilities.can_pop_frame = 1;
    capabilities.can_generate_frame_pop_events = 1;
    capabilities.can_redefine_classes = 1;
    capabilities.can_redefine_any_class = 1;
    capabilities.can_retransform_classes = 1;
    capabilities.can_retransform_any_class = 1;

    opResult = jEnv->AddCapabilities(&capabilities);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<< "Failed to enable event generation. Error code : " <<opResult<< "\n";
        return JNI_OK;
    }

    jvmEnvironment = jEnv;
    javaNativeInterface = jni;
    javaVm = vm;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit = &callbackVMInit;
    callbacks.Breakpoint = &callbackBreakpoint;
    callbacks.MethodEntry = &callbackMethodEntry;
    callbacks.MethodExit = &callbackMethodExit;
    callbacks.Exception = &callbackException;

    opResult = jEnv->SetEventCallbacks(&callbacks, (jint) sizeof(callbacks));
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to set event callbacks. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    opResult = jEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread) NULL);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to register VM_START event. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    opResult = jEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, (jthread) NULL);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to register Breakpoint event. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    opResult = jEnv->CreateRawMonitor("exceptionCallbackLock", &gblExceptionCallbackLock);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to create monitor. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    std::cout<< "Agent attached successfully" << "\n";
    return JNI_OK;
}

/**

Called by JVM at startup time when the agent is loaded.
This method initializes the jvmti environment and enables all the
capabilities.

**/
JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM* vm, char *options, void *reserved)
{
    jvmtiEnv *jEnv = NULL;
    JNIEnv *jni = NULL;
    jint opResult;

    opResult = vm->GetEnv((void **) &jEnv, JVMTI_VERSION_1_2);

    if(opResult != JNI_OK || jEnv == NULL)
    {
        std::cout << "Failed to get JVM environment, Error code : "<< opResult;
        return JNI_OK;
    }

    jvmtiCapabilities capabilities;
    memset(&capabilities, 0, sizeof(capabilities));
    jEnv->GetCapabilities(&capabilities);

    // enable event generation.
    capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_generate_method_exit_events = 1;
    capabilities.can_generate_method_entry_events = 1;
    capabilities.can_generate_exception_events = 1;
    capabilities.can_get_line_numbers = 1;
    capabilities.can_get_source_file_name = 1;
    capabilities.can_pop_frame = 1;
    capabilities.can_generate_frame_pop_events = 1;
    capabilities.can_redefine_classes = 1;
    capabilities.can_redefine_any_class = 1;
    capabilities.can_retransform_classes = 1;
    capabilities.can_retransform_any_class = 1;

    opResult = jEnv->AddCapabilities(&capabilities);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<< "Failed to enable event generation. Error code : " <<opResult<< "\n";
        return JNI_OK;
    }

    jvmEnvironment = jEnv;
    javaNativeInterface = jni;
    javaVm = vm;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit = &callbackVMInit;
    callbacks.Breakpoint = &callbackBreakpoint;
    callbacks.MethodEntry = &callbackMethodEntry;
    callbacks.MethodExit = &callbackMethodExit;
    callbacks.Exception = &callbackException;

    opResult = jEnv->SetEventCallbacks(&callbacks, (jint) sizeof(callbacks));
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to set event callbacks. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    opResult = jEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (jthread) NULL);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to register VM_START event. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    opResult = jEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, (jthread) NULL);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to register Breakpoint event. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    opResult = jEnv->CreateRawMonitor("exceptionCallbackLock", &gblExceptionCallbackLock);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<<"Failed to create monitor. Error code: : "<<opResult<<"\n";
        return JNI_OK;
    }

    std::cout<< "Agent attached successfully" << "\n";
    return JNI_OK;
}

void Agent_OnUnload(JavaVM *vm)
{
}
