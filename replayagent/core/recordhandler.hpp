#pragma once
#include "global_includes.hpp"

#ifndef __RECORD_DATA__HPP
#define __RECORD_DATA__HPP
#include "recorddata.hpp"
#endif // __RECORD_DATA__HPP

class RecordHandler
{
private:

    std::string threadExecutingRecord;
    jthread thread;
    int maxDepth = 0;

    std::unordered_map<jmethodID, std::list<RecordMethodData>> methodIdToDataMap;
    std::string convertJStringToCppString(JNIEnv *env, jstring methodName);
    jmethodID getMethodId(JNIEnv *jenv, std::string klass, std::string methodName);
    void addMethodDataToMap(std::string methodName, jvalue mReturnValue, char mReturnType, int depth, jmethodID methodId);

public:

    RecordHandler(std::string tName, jthread td)
    {
        threadExecutingRecord = tName;
        thread = td;
    }

    ~RecordHandler()
    {
        methodIdToDataMap.clear();
    }

    jthread getThread();
    std::string getThreadName();
    bool canMethodBeForcedToReturn(jmethodID method);
    RecordMethodData getMethodData(jmethodID method);
    void computeMaxDepth();
    void registerMethodRBoolean(JNIEnv *jenv, jstring methdName, jstring klassName, jboolean value, jint depth);
    void registerMethodRByte(JNIEnv *jenv, jstring methdName, jstring klassName, jbyte value, jint depth);
    void registerMethodRChar(JNIEnv *jenv, jstring methdName, jstring klassName, jchar value, jint depth);
    void registerMethodRShort(JNIEnv *jenv, jstring methdName, jstring klassName, jshort value, jint depth);
    void registerMethodRInt(JNIEnv *jenv, jstring methdName, jstring klassName, jint value, jint depth);
    void registerMethodRLong(JNIEnv *jenv, jstring methdName, jstring klassName, jlong value, jint depth);
    void registerMethodRFloat(JNIEnv *jenv, jstring methdName, jstring klassName, jfloat value, jint depth);
    void registerMethodRDouble(JNIEnv *jenv, jstring methdName, jstring klassName, jdouble value, jint depth);
    void registerMethodRVoid(JNIEnv *jenv, jstring methdName, jstring klassName, jint depth);
    void registerMethodRObject(JNIEnv *jenv, jstring methdName, jstring klassName, jobject value, jint depth);
};
