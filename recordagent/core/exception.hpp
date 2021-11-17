// Header imports
#include "global_includes.hpp"


/**
    Bridge to interact with java "Exception class"
**/
class Exception
{
    jclass klass;
    jmethodID exceptionCallbackMtdId;

public:
    /**
        Initializes by identifying whether class if found in JVM env.
    **/
    bool init();

    /**
        Exception callback made when the matching exception is raised.
    **/
    bool exceptionCallback(JNIEnv* jni_env, jobject thread, jobject exceptn, jstring className, jstring methodName);
};

// initialized in agent::start;
extern Exception *gblException;
