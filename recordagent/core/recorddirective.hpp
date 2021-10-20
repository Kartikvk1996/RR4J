// Header imports
#include "global_includes.hpp"

// Bridge to interact with java "RecordDirective class"

class RecordDirective
{
    jclass klass;
    jmethodID setDirectiveMtdId;
    jmethodID setExitOnExceptionMtdId;
    jmethodID setExceptionCaughtMtdId;
    jmethodID setRememberHistoryMtdId;
    jmethodID setThreadsToRecordMtdId;
    jmethodID makeRuleActiveMtdId;
    jmethodID isDumpCompletedMtdId;
    jmethodID initiateDumpingMtdId;
    jmethodID setRecordingStatusMtdId;
    jmethodID getRecordingStatusMtdId;
    jmethodID resetDirectivesMtdId;

public:
    bool init();
    void setDirectives(bool exitOnException, int rememberHistory, int threadsToRecord, bool activeRuleSet, jstring expn, int serializeThisObj);
    void setExitOnException(bool value);
    void setExceptionCaught(bool value);
    void setRememberHistory(int value);
    void setThreadsToRecord(int value);
    void makeRuleActive(bool value);
    bool isDumpCompleted();
    void initiateDumping();
    void setRecordingStatus(int value);
    // callbacks will pass their own jni variable and we should use that else we get lock error.
    void setRecordingStatus(JNIEnv* jniEnv, int value);
    int getRecordingStatus();
    void resetDirectives();
};

// initialized in agent::start()
extern RecordDirective *gblRecordDirective;
