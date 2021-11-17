/**
    Handles the callback raised by JVM. For any callbacks except VMInit to be handled. there must be a active rule set,
    else all callbacks are ignored and JVM will continue to run normally.
**/

/**
    Callback received when the VM is initialized loading all classes and ready for execution.
    During this phase we initialize the agent and start the same on new thread.
**/
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

/**
    Callback received when an exception is raised.
**/
static void JNICALL callbackException(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method, jlocation location, jobject excep, jmethodID catch_method, jlocation catch_location)
{
    Rule* rule = gblAgent->getCongiguration()->getActiveRule();

    // Exit on exception is not set, or exception is received in the method not traced. we ignore it.
    if((rule->isExceptionSet()==false) || (rule->getTracedMethodsList().find(method) == rule->getTracedMethodsList().end()))
    {
        return;
    }
    else // Check if exception matches, if yes, collect all the details and propagate to Java layer to handle it.
    {
        jvmti_env->RawMonitorEnter(gblExceptionCallbackLock);

        // Exception originating from depth > recorded depth
        auto entry = rule->getTracedMethodsList().find(method);
        if(entry->second == rule->getDepth())
        {
            std::string mName;
            std::unordered_map<std::string, jmethodID>& nameIdMap = rule->getMethodNameIdMap();
            for(auto it = nameIdMap.begin(); it != nameIdMap.end(); it++)
            {
                if(it->second == method)
                {
                    mName = it->first;
                    break;
                }
            }

            jclass methodDeclaringClass;
            jvmtiError error = jvmti_env->GetMethodDeclaringClass(method, &methodDeclaringClass);
            if(!Agent::checkErrorAndLog(error))
            {
                char* classSignaturePtr;
                char* classGenericPtr;
                error = jvmti_env->GetClassSignature(methodDeclaringClass, &classSignaturePtr, &classGenericPtr);
                jni_env->DeleteLocalRef(methodDeclaringClass);
                if(!Agent::checkErrorAndLog(error))
                {
                    jstring className = jni_env->NewStringUTF(classSignaturePtr);
                    jstring methodName = jni_env->NewStringUTF(mName.c_str());
                    PLOGI<<"Exception got originated from method "<<mName;
                    if(gblException->exceptionCallback(jni_env, thread, excep, className, methodName) == true)
                    {
                        if(rule->isExitOnException() == true)
                        {
                            PLOGI<<"Exception handled, exitOnException is true, hence stopping exception and initiating record dumping process";
                            gblRecordDirective->setRecordingStatus(jni_env, 2);
                        }
                    }
                    Agent::memDeAllocate(classSignaturePtr);
                    Agent::memDeAllocate(classGenericPtr);
                }
            }
        }

        jvmti_env->RawMonitorExit(gblExceptionCallbackLock);
    }
}

/**
    Callback received when a interested thread enters the method.
**/
static void JNICALL callbackMethodEntry(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method)
{
    Rule* rule = gblAgent->getCongiguration()->getActiveRule();

    // Store the method information if method lies within max configured depth limit.
    if(rule->getCurrentDepth() <= rule->getDepth())
    {
        rule->getTracedMethodsList().insert(std::make_pair(method, rule->getCurrentDepth()));
    }
    rule->incrementCurrentDepth();
}

/**
    Callback received when a interested thread exits the method.
**/
static void JNICALL callbackMethodExit(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method, jboolean was_popped_by_exception, jvalue return_value)
{
    Rule* rule = gblAgent->getCongiguration()->getActiveRule();

    if(rule->getCurrentDepth()>0)
        rule->decrementCurrentDepth();

    /*
        If we are at the starting method call then stop all the method entry and exit events on the thread and detach the hook and move to
        method transform phase.
    */
    if(rule->getCurrentDepth() == 0)
    {
        // Disable method entry event.
        jvmtiError error = jvmti_env->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_ENTRY, thread);
        if(!Agent::checkErrorAndLog(error))
        {
            // Disable method exit event.
            error = jvmti_env->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_EXIT, thread);
            if(!Agent::checkErrorAndLog(error))
            {
                jvmtiThreadInfo threadInfo;
                jvmti_env->GetThreadInfo(thread, &threadInfo);
                PLOGI<<"Stopped tracing method calls on a thread : "<<threadInfo.name;
                Agent::memDeAllocate(threadInfo.name);
                rule->setTracingCompleted();
            }
        }
        rule->getMethod()->clearAllHooks();
        rule->decrementThreadsToMonitor();
    }
}

/**
    Callback received when attached hook is triggered.
**/
static void JNICALL callbackBreakpoint(jvmtiEnv *jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method, jlocation location)
{
    Rule* rule = gblAgent->getCongiguration()->getActiveRule();
    if(rule == NULL)
        return;

    if(rule->isMethodHooksAttached() && (rule->isThreadsMonitorMaxedOut() == false))
    {
        if(rule->incrementThreadsToMonitor())
        {
            // Trace method entries for the give thread;
            jvmtiError error = jvmti_env->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, thread);
            if(!Agent::checkErrorAndLog(error))
            {
                error = jvmti_env->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_EXIT, thread);
                if(!Agent::checkErrorAndLog(error))
                {
                    rule->getTracedMethodsList().insert(std::make_pair(method, rule->getCurrentDepth()));
                    rule->incrementCurrentDepth();
                    jvmtiThreadInfo threadInfo;
                    jvmti_env->GetThreadInfo(thread, &threadInfo);
                    PLOGI<<"Started tracing method calls on a thread : "<<threadInfo.name;
                    Agent::memDeAllocate(threadInfo.name);
                }

            }
        }
    }
}


