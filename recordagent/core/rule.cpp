#ifndef __RULE__HPP
#define __RULE__HPP
#include "rule.hpp"
#endif // __RULE__HPP

#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

#ifndef __RECORD_DIRECTIVE__HPP
#define __RECORD_DIRECTIVE__HPP
#include "recorddirective.hpp"
#endif // __RECORD_DIRECTIVE__HPP

#ifndef __SERIALIZER__HPP
#define __SERIALIZER__HPP
#include "serializer.hpp"
#endif // __SERIALIZER__HPP

std::string Rule::getMethodPoint()
{
    return methodPoint;
}

std::string Rule::getCatchException()
{
    return catchException;
}

bool Rule::isAdditivity()
{
    return additivity;
}

bool Rule::isExitOnException()
{
    return exitOnException;
}

std::unordered_map<jmethodID, int>& Rule::getTracedMethodsList()
{
    return tracedMethods;
}

bool Rule::init()
{
    classTransformer = javaNativeInterface->FindClass("com/recordreplay/record/transform/ClassTransformer");
	if (classTransformer == NULL)
    {
        PLOGE<<"Failed to find 'com.recordreplay.record.transform.ClassTransformer' class.";
        return false;
	}

	transformClassMtdId = javaNativeInterface->GetStaticMethodID(classTransformer, "transformClass", "(Ljava/lang/Class;Ljava/lang/String;I)[B");
	if (transformClassMtdId == NULL)
    {
        PLOGE<<"Failed to find 'com.recordreplay.record.transform.ClassTransformer::transformClass' method.";
        return false;
	}

	getMtdNameToRestoreMtdId = javaNativeInterface->GetStaticMethodID(classTransformer, "getMtdNameToRestore", "()Ljava/lang/String;");
	if (getMtdNameToRestoreMtdId == NULL)
    {
        PLOGE<<"Failed to find 'com.recordreplay.record.transform.ClassTransformer::getMtdNameToRestore' method.";
        return false;
	}

	getByteCodeToRestoreMtdId = javaNativeInterface->GetStaticMethodID(classTransformer, "getByteCodeToRestore", "()[B");
	if (getByteCodeToRestoreMtdId == NULL)
    {
        PLOGE<<"Failed to find 'com.recordreplay.record.transform.ClassTransformer::getByteCodeToRestore' method.";
        return false;
	}

	return true;
}

bool Rule::incrementThreadsToMonitor()
{
    try
    {
        std::lock_guard<std::mutex> lock(threadsMonitoredCountLock);
        if(threadsMonitoredSoFar >= maxThreadsToMonitor)
        {
            threadsMonitoredSoFar = maxThreadsToMonitor;
            PLOGW<< "Can't add thread to monitor, counter maxed out" << "Max = "<< maxThreadsToMonitor <<" Current = "<<threadsMonitoredSoFar;
            return false;
        }
        else
        {
            threadsMonitoredSoFar++;
            PLOGI<< "New thread added and will be monitored";
            return true;
        }
    }
    catch(std::exception& e)
    {
        PLOGE<< "Exception occurred while adding thread to monitor. "<< e.what();
    }
    return false;
}

bool Rule::decrementThreadsToMonitor()
{
    try
    {
        std::lock_guard<std::mutex> lock(threadsMonitoredCountLock);
        if(threadsMonitoredSoFar <=0)
        {
            threadsMonitoredSoFar = 0;
            PLOGW<< "Can't remove thread to monitor, no threads are monitored"<< "Max = "<< maxThreadsToMonitor <<" Current = "<<threadsMonitoredSoFar;
            return false;
        }
        else
        {
            threadsMonitoredSoFar--;
            PLOGI<< "Thread removed and will not be monitored";
            return true;
        }
    }
    catch(std::exception& e)
    {
        PLOGE<< "Exception occurred while removing thread which is monitored. "<< e.what();
    }
    return false;
}

bool Rule::isThreadsMonitorMaxedOut()
{
    try
    {
        std::lock_guard<std::mutex> lock(threadsMonitoredCountLock);
        if(threadsMonitoredSoFar >= maxThreadsToMonitor)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    catch(std::exception& e)
    {
        PLOGE<< "Exception occurred. "<< e.what();
    }
    return false;
}

int Rule::getDepth()
{
    return depth;
}

bool Rule::isTracingCompleted()
{
    return tracingCompleted;
}

bool Rule::setTracingCompleted()
{
    std::lock_guard<std::mutex> lock(tracingCompleteLock);
    tracingCompleted = true;
    return tracingCompleted;
}

bool Rule::setTransformationDone()
{
    std::lock_guard<std::mutex> lock(transformationCompleteLock);
    transformationDone = true;
    return transformationDone;
}


bool Rule::isTransformationDone()
{
    return transformationDone;
}


int Rule::getThreadsToMonitor()
{

}

int Rule::getCurrentDepth()
{
    return currentDepth;
}

int Rule::incrementCurrentDepth()
{
    currentDepth++;
    return currentDepth;
}

int Rule::decrementCurrentDepth()
{
    currentDepth--;
    return currentDepth;
}

void Rule::setMethod(std::shared_ptr<Method> m)
{
    method = m;
}

std::shared_ptr<Method> Rule::getMethod()
{
    return method;
}

void Rule::start()
{
    if(isTracingCompleted() == false)
        identifyMethodCalls();
    else
        PLOGE<<"Skipping Tracing Method calls.";

    if(isTransformationDone() == false)
        startTransformation();
    else
        PLOGE<<"Skipping method Transformation.";

    if(gblRecordDirective->getRecordingStatus() == 1)
    {
        while(gblRecordDirective->getRecordingStatus() == 1)
        {
            PLOGI<<"Waiting for record to complete";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        jvmtiError error = jvmEnvironment->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_EXCEPTION, (jthread) NULL);
        Agent::checkErrorAndLog(error);
    }
    else
        PLOGE<<"Skipping method Recording";

    if(gblRecordDirective->getRecordingStatus() == 2)
        initiateDumpAndWait();
    else
        PLOGE<<"Skipping Record dump creation";

    // Safe side always detransform the code.
    startDeTransformation();
}

void Rule::identifyMethodCalls()
{
    PLOGI<<"Identifying method calls for Rule with Method "<<methodPoint<<", Exception : "<<catchException;
    if(method->addHook(methodPoint))
    {
        setMethodHooksAttached();
        while(isTracingCompleted() == false)
        {
            PLOGI<<"Waiting for a tracing to complete.";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        if(isExceptionSet() == true)
        {
            PLOGI<<"Watching for '"<<getCatchException()<<"' Exception";
            jvmtiError error = jvmEnvironment->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, (jthread) NULL);
            if(Agent::checkErrorAndLog(error))
            {
                PLOGE<<"Fatal error in watching for '"<<getCatchException()<<"' Exception";
                setTracingCompleted();
                setTransformationDone();
            }
        }
    }
    else
    {
        PLOGE<<"Fatal error identifying method calls.";
        setTracingCompleted();
        setTransformationDone();
    }
}

void Rule::startTransformation()
{
    PLOGI<<"Tracing Completed. Will start the Bytecode transformation on below methods.";

	bool allMethodsTransformed = true;
	bool fatalError = false;
    for(std::pair<jmethodID, int> method : tracedMethods)
    {
        if(fatalError == true)
            break;

        char* mName;
        char* mSignature;
        char* mGenericPtr;
        jvmtiError error = jvmEnvironment->GetMethodName(method.first, &mName, &mSignature, &mGenericPtr);
        if(!Agent::checkErrorAndLog(error))
        {
            jclass klass;
            error = jvmEnvironment->GetMethodDeclaringClass(method.first, &klass);
            if(!Agent::checkErrorAndLog(error))
            {
                char *klassSignature;
                char *klassGeneric;
                error = jvmEnvironment->GetClassSignature(klass, &klassSignature, &klassGeneric);
                if(!Agent::checkErrorAndLog(error))
                {
                    if(isPackageBlacklisted(klassSignature))
                    {
                        Agent::memDeAllocate(klassSignature);
                        Agent::memDeAllocate(klassGeneric);
                        Agent::memDeAllocate(mName);
                        Agent::memDeAllocate(mSignature);
                        Agent::memDeAllocate(mGenericPtr);
                        continue;
                    }


                    PLOGI<<"Method will be instrumented --> "<<klassSignature<<mName<<mSignature;

                    std::string methodName = std::string(mName);
                    methodName+=" ";
                    methodName+=mSignature;
                    jstring arg2 = javaNativeInterface->NewStringUTF(methodName.c_str());

                    jbyteArray result = (jbyteArray)javaNativeInterface->CallStaticObjectMethod(classTransformer, transformClassMtdId, klass, arg2, method.second);
                    if(result != 0)
                    {
                        PLOGI<<"New Transformed bytecode received for method '"<<methodName<<"', and will be injected.";
                        jboolean isCopy;
                        jbyte *classByteData = javaNativeInterface->GetByteArrayElements(result, &isCopy);
                        jvmtiClassDefinition newDefinition;
                        newDefinition.klass = klass;
                        newDefinition.class_byte_count = javaNativeInterface->GetArrayLength(result);
                        newDefinition.class_bytes = (unsigned char *)classByteData;

                        error = jvmEnvironment->RedefineClasses(1, &newDefinition);
                        if(!Agent::checkErrorAndLog(error))
                        {
                            methodNameIdMap.insert(std::make_pair(methodName, method.first));
                            PLOGI<<"BCI for class "<<klassSignature<<mName<<mSignature<<" is successful.";
                            javaNativeInterface->DeleteLocalRef(result);
                        }
                        else
                        {
                            allMethodsTransformed = false;
                            PLOGI<<"BCI for class "<<klassSignature<<mName<<mSignature<<" failed.";
                        }

                        if(classByteData != NULL);
                            //javaNativeInterface->ReleaseByteArrayElements(result, classByteData, JNI_ABORT);
                    }
                    else
                    {
                        PLOGE<<"Fatal error while transforming method '"<<methodName<<"', didn't receive new bytecode";
                        fatalError = true;
                    }

                    Agent::memDeAllocate(klassSignature);
                    Agent::memDeAllocate(klassGeneric);
                }
            }

            Agent::memDeAllocate(mName);
            Agent::memDeAllocate(mSignature);
            Agent::memDeAllocate(mGenericPtr);
        }
    }

    if(allMethodsTransformed == true && fatalError == false)
    {
        PLOGI<<"All methods are transformed, starting recording";
        gblRecordDirective->setRecordingStatus(1);
    }

}

void Rule::startDeTransformation()
{
    PLOGI<<"Restoring to old bytecode to restore the old behavior";

	bool restoredAtleastOneClass = false;
    while(1)
    {
        jbyteArray result = (jbyteArray)javaNativeInterface->CallStaticObjectMethod(classTransformer, getByteCodeToRestoreMtdId);
        if(result != 0)
        {
            restoredAtleastOneClass = true;
            jstring mName = (jstring)javaNativeInterface->CallStaticObjectMethod(classTransformer, getMtdNameToRestoreMtdId);
            const char* methodName = javaNativeInterface->GetStringUTFChars(mName, NULL);
            std::string temp = methodName;
            try
            {
                jmethodID mId = methodNameIdMap.at(temp);
                jclass klass;
                jvmtiError error = jvmEnvironment->GetMethodDeclaringClass(mId, &klass);
                if(!Agent::checkErrorAndLog(error))
                {
                    jboolean isCopy;
                    jbyte *classByteData = javaNativeInterface->GetByteArrayElements(result, &isCopy);
                    jvmtiClassDefinition oldDefinition;
                    oldDefinition.klass = klass;
                    oldDefinition.class_byte_count = javaNativeInterface->GetArrayLength(result);
                    oldDefinition.class_bytes = (unsigned char *)classByteData;

                    error = jvmEnvironment->RedefineClasses(1, &oldDefinition);
                    if(!Agent::checkErrorAndLog(error))
                    {
                        PLOGI<<"Successfully restored method '"<<temp<<"', which was instrumented earlier";
                        javaNativeInterface->DeleteLocalRef(result);
                    }
                    else
                    {
                        PLOGE<<"Failed to restore method '"<<temp<<"', which was instrumented earlier";
                    }

                    if(classByteData != NULL);
                        //javaNativeInterface->ReleaseByteArrayElements(result, classByteData, JNI_ABORT);
                }
                else
                {
                    PLOGE<<"Failed to get declaring class for method '"<<temp<<"'";
                }
            }
            catch(std::out_of_range &e)
            {
                PLOGE<<"Method '"<<temp<<"' was never instrumented";
            }

            if(methodName != NULL)
                javaNativeInterface->ReleaseStringUTFChars(mName, methodName);
        }
        else
        {
            break;
        }
    }

    if(restoredAtleastOneClass == false)
        PLOGI<<"No bytecode modification was done, nothing to restore";
}

void Rule::initiateDumpAndWait()
{
    std::string loc = gblAgent->getCongiguration()->getRecordDumpLocation();
    PLOGI<<"Initiating record dumping process, dump file will be generated at location : "<<loc;
    gblRecordDirective->initiateDumping();
    bool status = gblSerializer->serializeData(loc);
    if(status == true)
    {
        PLOGI<<"Record dumping process completed, location : "<<loc;
    }
    else
    {
        PLOGE<<"Error occurred while creating dump file";
    }
}

void Rule::cleanup()
{
    // Cleanup can happen before method is initialized.
    if(method.use_count() == 0)
        return;
    method->clearAllHooks();
}

int Rule::getHistory()
{
    return history;
}

int Rule::getThreadsToRecord()
{
    return threadsToRecord;
}

bool Rule::isExceptionSet()
{
    if(catchException.length() > 0)
        return true;
    else
        return false;
}

std::unordered_map<std::string, jmethodID>& Rule::getMethodNameIdMap()
{
    return methodNameIdMap;
}

bool Rule::isMethodHooksAttached()
{
    return isMtdHooksAttached;
}

bool Rule::setMethodHooksAttached()
{
    isMtdHooksAttached = true;
}

bool Rule::isPackageBlacklisted(char* klassSignature)
{
    std::vector<std::regex>& blp = gblAgent->getCongiguration()->getBlacklistPackages();
    for(std::regex r : blp)
    {
        if(std::regex_match(klassSignature, r))
        {
            PLOGI<<"Package containing class '"<<klassSignature<<"' is blacklisted, skipping instrumentation";
            return true;
        }
    }
}
