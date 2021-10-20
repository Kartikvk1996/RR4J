struct NativeFunctions
{
    void (*NativeCall_registerThread)(JNIEnv*, jclass , jstring, jthread);
    void (*NativeCall_deregisterThread)(JNIEnv*, jclass , jstring, jthread);
    void (*NativeCall_startMonitoring)(JNIEnv*, jclass , jstring, jthread);
    void (*NativeCall_stopMonitoring)(JNIEnv*, jclass , jstring, jthread);
    void (*NativeCall_registerMethodRBoolean)(JNIEnv*, jclass , jstring, jstring, jstring, jboolean, jint);
    void (*NativeCall_registerMethodRByte)(JNIEnv*, jclass , jstring, jstring, jstring, jbyte, jint);
    void (*NativeCall_registerMethodRChar)(JNIEnv*, jclass , jstring, jstring, jstring, jchar, jint);
    void (*NativeCall_registerMethodRShort)(JNIEnv*, jclass , jstring, jstring, jstring, jshort, jint);
    void (*NativeCall_registerMethodRInt)(JNIEnv*, jclass , jstring, jstring, jstring, jint, jint);
    void (*NativeCall_registerMethodRLong)(JNIEnv*, jclass , jstring, jstring, jstring, jlong, jint);
    void (*NativeCall_registerMethodRFloat)(JNIEnv*, jclass , jstring, jstring, jstring, jfloat, jint);
    void (*NativeCall_registerMethodRDouble)(JNIEnv*, jclass , jstring, jstring, jstring, jdouble, jint);
    void (*NativeCall_registerMethodRVoid)(JNIEnv*, jclass , jstring, jstring, jstring, jint);
    void (*NativeCall_registerMethodRObject)(JNIEnv*, jclass , jstring, jstring, jstring, jobject, jint);
};
