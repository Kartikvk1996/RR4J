#ifndef __RECORD_HANDLER__HPP
#define __RECORD_HANDLER__HPP
#include "recordhandler.hpp"
#endif // __RECORD_HANDLER__HPP

#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

jthread RecordHandler::getThread()
{
    return thread;
}

std::string RecordHandler::getThreadName()
{
    return threadExecutingRecord;
}

void RecordHandler::registerMethodRBoolean(JNIEnv* jenv, jstring methdName, jstring klassName, jboolean value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.z = value;

    addMethodDataToMap(mName, returnValue, 'Z', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'boolean' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRByte(JNIEnv* jenv, jstring methdName, jstring klassName, jbyte value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.b = value;

    addMethodDataToMap(mName, returnValue, 'B', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'byte' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRChar(JNIEnv* jenv, jstring methdName, jstring klassName, jchar value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.c = value;

    addMethodDataToMap(mName, returnValue, 'C', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'char' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRShort(JNIEnv* jenv, jstring methdName, jstring klassName, jshort value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.s = value;

    addMethodDataToMap(mName, returnValue, 'S', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'short' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRInt(JNIEnv* jenv, jstring methdName, jstring klassName, jint value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.i = value;

    addMethodDataToMap(mName, returnValue, 'I', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'int' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRLong(JNIEnv* jenv, jstring methdName, jstring klassName, jlong value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.j = value;

    addMethodDataToMap(mName, returnValue, 'J', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'long' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRFloat(JNIEnv* jenv, jstring methdName, jstring klassName, jfloat value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.f = value;

    addMethodDataToMap(mName, returnValue, 'F', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'float' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRDouble(JNIEnv* jenv, jstring methdName, jstring klassName, jdouble value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.d = value;

    addMethodDataToMap(mName, returnValue, 'D', depth, mtdId);

    PLOGI<<"Registered method '"<<mName<<"' of return type 'double' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRObject(JNIEnv* jenv, jstring methdName, jstring klassName, jobject value, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));
    returnValue.l = jenv->NewGlobalRef(value);

    addMethodDataToMap(mName, returnValue, 'L', depth, mtdId);

    PLOGI<<"Registered method '"<<mName<<"' of return type 'object' "<<value<<" "<<depth;
}

void RecordHandler::registerMethodRVoid(JNIEnv* jenv, jstring methdName, jstring klassName, jint depth)
{
    std::string mName = convertJStringToCppString(jenv, methdName);

    jmethodID mtdId = getMethodId(jenv, convertJStringToCppString(jenv, klassName), mName);
    if(mtdId == NULL)
    {
        PLOGI<<"Failed to register method '"<<mName<<"'";
        return;
    }

    jvalue returnValue;
    memset(&returnValue, 0, sizeof(returnValue));

    addMethodDataToMap(mName, returnValue, 'V', depth, mtdId);
    PLOGI<<"Registered method '"<<mName<<"' of return type 'object' "<<" "<<depth;
}

std::string RecordHandler::convertJStringToCppString(JNIEnv *env, jstring value)
{
    const char *mName = env->GetStringUTFChars(value, 0);
    std::string temp = mName;
    env->ReleaseStringUTFChars(value, mName);
    return temp;
}

jmethodID RecordHandler::getMethodId(JNIEnv *env, std::string klass, std::string methodName)
{
    std::replace( klass.begin(), klass.end(), '.', '/');
    jclass kls = env->FindClass(klass.c_str());
    if(kls == NULL)
    {
        PLOGE <<"Failed to fetch the class. Name : "<<klass;
        return NULL;
    }

    PLOGI << "Fetched class = '"<<klass<<"' from running JVM";

    jmethodID temp = NULL;
    jint methodCtr;
    jmethodID *methodPtr = NULL;
    jvmtiError error = jvmEnvironment->GetClassMethods(kls, &methodCtr, &methodPtr);
    if(!Agent::checkErrorAndLog(error))
    {
        PLOGI<< "Fetched method list for a given class :"<<klass;
        bool isMethodFound = false;
        for(jint ittr = 0; ittr < methodCtr; ittr++)
        {
            char* mName;
            char* mSignature;
            char* mGenericPtr;
            error = jvmEnvironment->GetMethodName(methodPtr[ittr], &mName, &mSignature, &mGenericPtr);
            if(!Agent::checkErrorAndLog(error))
            {
                if(isMethodFound == false)
                {
                    std::string obtainedMethodName = std::string(mName);
                    obtainedMethodName+=" ";
                    obtainedMethodName+=mSignature;
                    if(obtainedMethodName.compare(methodName) == 0)
                    {
                        temp = methodPtr[ittr];
                        isMethodFound = true;
                    }
                }
                Agent::memDeAllocate(mName);
                Agent::memDeAllocate(mSignature);
                Agent::memDeAllocate(mGenericPtr);
            }

            if(isMethodFound == true)
                break;
        }
        Agent::memDeAllocate(methodPtr);
    }
    return temp;
}

void RecordHandler::computeMaxDepth()
{
    // Not needed we can do the same while adding the method
//    for(auto it = methodIdToDataMap.begin(); it != methodIdToDataMap.end(); it++)
//    {
//        if(maxDepth < it->second.depth)
//        {
//            maxDepth = it->second.depth;
//        }
//    }
    PLOGI<<"Max depth configured to value : "<<maxDepth;
}

bool RecordHandler::canMethodBeForcedToReturn(jmethodID method)
{
    if(methodIdToDataMap.find(method) != methodIdToDataMap.end())
    {
        std::list<RecordMethodData>& mList = methodIdToDataMap[method];
        RecordMethodData data = mList.front();
        if(data.methodId == method)
        {
            if(data.depth == maxDepth)
            {
                PLOGI<<"Method '"<<data.methodName<<"', methodID '"<<data.methodId<<"' call will be impersonated";
                return true;
            }
        }
    }
    return false;
}

RecordMethodData RecordHandler::getMethodData(jmethodID method)
{
    if(methodIdToDataMap.find(method) != methodIdToDataMap.end())
    {
        std::list<RecordMethodData>& mList = methodIdToDataMap[method];
        RecordMethodData data = mList.front();
        mList.pop_front();
        return data;
    }
    else
    {
        PLOGE<<"Method with methodID '"<<method<<"' was not registered";
    }
    // this control should never reach
    RecordMethodData data;
    data.depth = -1;
    data.methodName = "God knows";
    data.methodType = 'U';
    return data;
}


void RecordHandler::addMethodDataToMap(std::string methodName, jvalue mReturnValue, char mReturnType, int depth, jmethodID methodId)
{
    if(maxDepth < depth)
    {
        maxDepth = depth;
    }

    RecordMethodData data;

    data.depth = depth;
    data.methodName = methodName;
    data.methodType = mReturnType;
    data.rValue = mReturnValue;
    data.methodId = methodId;

    if(methodIdToDataMap.find(methodId) == methodIdToDataMap.end())
    {
        std::list<RecordMethodData> mList;
        mList.push_back(data);
        methodIdToDataMap.insert(std::make_pair(methodId, mList));
    }
    else
    {
        std::list<RecordMethodData>& mList = methodIdToDataMap[methodId];
        mList.push_back(data);
    }
    return;
}
