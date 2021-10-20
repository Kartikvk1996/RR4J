/**
Main JVM agent loaded at runtime.
**/

// Import all headers files.
#include "core/global_includes.hpp"

#ifndef __NATIVE_FUNCTIONS__HPP
#define __NATIVE_FUNCTIONS__HPP
#include "core/nativefunctions.hpp"
#endif // __NATIVE_FUNCTIONS_HPP

#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "core/agent.hpp"
#endif // __AGENT_HPP

jvmtiEnv *jvmEnvironment;
JNIEnv *javaNativeInterface;
jvmtiEventCallbacks callbacks;
NativeFunctions nativeFunctions;
Agent *gblAgent;

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

    jvmtiCapabilities capabilities;
    memset(&capabilities, 0, sizeof(capabilities));

    // enable event generation.
    //capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_generate_method_exit_events = 1;
    capabilities.can_generate_method_entry_events = 1;
    //capabilities.can_generate_exception_events = 1;
    //capabilities.can_get_line_numbers = 1;
    //capabilities.can_get_source_file_name = 1;
    capabilities.can_pop_frame = 1;
    capabilities.can_generate_frame_pop_events = 1;
    //capabilities.can_redefine_classes = 1;
    //capabilities.can_retransform_classes = 1;
    capabilities.can_force_early_return = 1;

    opResult = jEnv->AddCapabilities(&capabilities);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<< "Failed to enable event generation. Error code : " <<opResult<< "\n";
        return JNI_OK;
    }

    jvmEnvironment = jEnv;
    javaNativeInterface = jni;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit = &callbackVMInit;
    callbacks.MethodEntry = &callbackMethodEntry;
    callbacks.MethodExit = &callbackMethodExit;

    nativeFunctions.NativeCall_registerThread = &NativeCall_registerThread;
    nativeFunctions.NativeCall_deregisterThread = &NativeCall_deregisterThread;
    nativeFunctions.NativeCall_startMonitoring = &NativeCall_startMonitoring;
    nativeFunctions.NativeCall_stopMonitoring = &NativeCall_stopMonitoring;
    nativeFunctions.NativeCall_registerMethodRBoolean = &NativeCall_registerMethodRBoolean;
    nativeFunctions.NativeCall_registerMethodRByte = &NativeCall_registerMethodRByte;
    nativeFunctions.NativeCall_registerMethodRChar = &NativeCall_registerMethodRChar;
    nativeFunctions.NativeCall_registerMethodRShort = &NativeCall_registerMethodRShort;
    nativeFunctions.NativeCall_registerMethodRInt = &NativeCall_registerMethodRInt;
    nativeFunctions.NativeCall_registerMethodRLong = &NativeCall_registerMethodRLong;
    nativeFunctions.NativeCall_registerMethodRFloat = &NativeCall_registerMethodRFloat;
    nativeFunctions.NativeCall_registerMethodRDouble = &NativeCall_registerMethodRDouble;
    nativeFunctions.NativeCall_registerMethodRVoid = &NativeCall_registerMethodRVoid;
    nativeFunctions.NativeCall_registerMethodRObject = &NativeCall_registerMethodRObject;

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

    // enable event generation.
    // enable event generation.
    //capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_generate_method_exit_events = 1;
    capabilities.can_generate_method_entry_events = 1;
    //capabilities.can_generate_exception_events = 1;
    //capabilities.can_get_line_numbers = 1;
    //capabilities.can_get_source_file_name = 1;
    capabilities.can_pop_frame = 1;
    capabilities.can_generate_frame_pop_events = 1;
    //capabilities.can_redefine_classes = 1;
    //capabilities.can_retransform_classes = 1;
    capabilities.can_force_early_return = 1;

    opResult = jEnv->AddCapabilities(&capabilities);
    if(opResult != JVMTI_ERROR_NONE)
    {
        std::cout<< "Failed to enable event generation. Error code : " <<opResult<< "\n";
        return JNI_OK;
    }

    jvmEnvironment = jEnv;
    javaNativeInterface = jni;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.VMInit = &callbackVMInit;
    callbacks.MethodEntry = &callbackMethodEntry;
    callbacks.MethodExit = &callbackMethodExit;

    nativeFunctions.NativeCall_registerThread = &NativeCall_registerThread;
    nativeFunctions.NativeCall_deregisterThread = &NativeCall_deregisterThread;
    nativeFunctions.NativeCall_startMonitoring = &NativeCall_startMonitoring;
    nativeFunctions.NativeCall_stopMonitoring = &NativeCall_stopMonitoring;
    nativeFunctions.NativeCall_registerMethodRBoolean = &NativeCall_registerMethodRBoolean;
    nativeFunctions.NativeCall_registerMethodRByte = &NativeCall_registerMethodRByte;
    nativeFunctions.NativeCall_registerMethodRChar = &NativeCall_registerMethodRChar;
    nativeFunctions.NativeCall_registerMethodRShort = &NativeCall_registerMethodRShort;
    nativeFunctions.NativeCall_registerMethodRInt = &NativeCall_registerMethodRInt;
    nativeFunctions.NativeCall_registerMethodRLong = &NativeCall_registerMethodRLong;
    nativeFunctions.NativeCall_registerMethodRFloat = &NativeCall_registerMethodRFloat;
    nativeFunctions.NativeCall_registerMethodRDouble = &NativeCall_registerMethodRDouble;
    nativeFunctions.NativeCall_registerMethodRVoid = &NativeCall_registerMethodRVoid;
    nativeFunctions.NativeCall_registerMethodRObject = &NativeCall_registerMethodRObject;

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

    std::cout<< "Agent attached successfully" << "\n";
    return JNI_OK;
}

void Agent_OnUnload(JavaVM *vm)
{

}
