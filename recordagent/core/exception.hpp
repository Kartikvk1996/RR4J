// Header imports
#include "global_includes.hpp"

// Bridge to interact with java "Exception class"

class Exception
{
    jclass klass;
    jmethodID exceptionCallbackMtdId;

public:
    bool init();
    bool exceptionCallback(JNIEnv* jni_env, jobject thread, jobject exceptn, jstring className, jstring methodName);
};

// initialized in agent::start;
extern Exception *gblException;
