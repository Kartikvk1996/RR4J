
#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

#ifndef __RECORD_DIRECTIVE__HPP
#define __RECORD_DIRECTIVE__HPP
#include "recorddirective.hpp"
#endif // __RECORD_DIRECTIVE__HPP


bool RecordDirective::init()
{
    klass = javaNativeInterface->FindClass("com/rr4j/record/RecordDirectives");
    if(klass == NULL)
    {
        PLOGE<<"Failed to find class 'com.rr4j.record.RecordDirectives'";
        return false;
    }

    setDirectiveMtdId = javaNativeInterface->GetStaticMethodID(klass, "setDirectives", "(ZIIZLjava/lang/String;Z)V");
    if(setDirectiveMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::setDirectives'";
        return false;
    }

    setExitOnExceptionMtdId = javaNativeInterface->GetStaticMethodID(klass, "setExitOnException", "(Z)V");
    if(setExitOnExceptionMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::setExitOnException'";
        return false;
    }

    setExceptionCaughtMtdId = javaNativeInterface->GetStaticMethodID(klass, "setExceptionCaught", "(Z)V");
    if(setExceptionCaughtMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::setExceptionCaught'";
        return false;
    }

    setRememberHistoryMtdId = javaNativeInterface->GetStaticMethodID(klass, "setRememberHistory", "(I)V");
    if(setRememberHistoryMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::setRememberHistory'";
        return false;
    }

    setThreadsToRecordMtdId = javaNativeInterface->GetStaticMethodID(klass, "setThreadsToRecord", "(I)V");
    if(setThreadsToRecordMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::setThreadsToRecord'";
        return false;
    }

    makeRuleActiveMtdId = javaNativeInterface->GetStaticMethodID(klass, "makeRuleActive", "(Z)V");
    if(makeRuleActiveMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::makeRuleActive'";
        return false;
    }

    isDumpCompletedMtdId = javaNativeInterface->GetStaticMethodID(klass, "isDumpCompleted", "()Z");
    if(isDumpCompletedMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::isDumpCompleted'";
        return false;
    }

    initiateDumpingMtdId = javaNativeInterface->GetStaticMethodID(klass, "initiateDumping", "()V");
    if(initiateDumpingMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::initiateDumping'";
        return false;
    }

    setRecordingStatusMtdId = javaNativeInterface->GetStaticMethodID(klass, "setRecordingStatus", "(I)V");
    if(setRecordingStatusMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::setRecordingStatus'";
        return false;
    }

    getRecordingStatusMtdId = javaNativeInterface->GetStaticMethodID(klass, "getRecordingStatus", "()I");
    if(getRecordingStatusMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::getRecordingStatus'";
        return false;
    }

    resetDirectivesMtdId = javaNativeInterface->GetStaticMethodID(klass, "resetDirectives", "()V");
    if(resetDirectivesMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::resetDirectives'";
        return false;
    }

    return true;
}

void RecordDirective::setDirectives(bool exitOnException, int rememberHistory, int threadsToRecord, bool activeRuleSet, jstring excpn, int serializeThisObj)
{
    PLOGI<<"Setting Record Directives, EXIT_ON_EXCEPTION : "<<exitOnException<<", REMEMBER_HISTORY : "<<rememberHistory<<", THREADS_TO_RECORD : "<<threadsToRecord<<", IS_ACTIVE_RULE_SET : "<<activeRuleSet;
    javaNativeInterface->CallStaticVoidMethod(klass, setDirectiveMtdId, exitOnException, rememberHistory, threadsToRecord, activeRuleSet, excpn, serializeThisObj);
}

void RecordDirective::setExitOnException(bool value)
{
    javaNativeInterface->CallStaticVoidMethod(klass, setExitOnExceptionMtdId, value);
}

void RecordDirective::setExceptionCaught(bool value)
{
    PLOGI<<"Exception caught, setting Record directive value = "<<value;
    javaNativeInterface->CallStaticVoidMethod(klass, setExceptionCaughtMtdId, value);
}

void RecordDirective::setRememberHistory(int value)
{
    javaNativeInterface->CallStaticVoidMethod(klass, setRememberHistoryMtdId, value);
}

void RecordDirective::setThreadsToRecord(int value)
{
    javaNativeInterface->CallStaticVoidMethod(klass, setThreadsToRecordMtdId, value);
}

void RecordDirective::makeRuleActive(bool value)
{
    javaNativeInterface->CallStaticVoidMethod(klass, makeRuleActiveMtdId, value);
}

bool RecordDirective::isDumpCompleted()
{
    return (bool)javaNativeInterface->CallStaticBooleanMethod(klass, isDumpCompletedMtdId);
}

void RecordDirective::initiateDumping()
{
    javaNativeInterface->CallStaticVoidMethod(klass, initiateDumpingMtdId);
}

void RecordDirective::setRecordingStatus(int value)
{
    javaNativeInterface->CallStaticVoidMethod(klass, setRecordingStatusMtdId, value);
}

void RecordDirective::setRecordingStatus(JNIEnv* jniEnv, int value)
{
    jmethodID mtd = jniEnv->GetStaticMethodID(klass, "setRecordingStatus", "(I)V");
    if(mtd == NULL)
    {
        PLOGE<<"Failed to find method 'com.rr4j.record.RecordDirectives::setRecordingStatus' during callback";
        return;
    }
    jniEnv->CallStaticVoidMethod(klass, mtd, value);
}

int RecordDirective::getRecordingStatus()
{
    return (int)javaNativeInterface->CallStaticBooleanMethod(klass, getRecordingStatusMtdId);
}

void RecordDirective::resetDirectives()
{
    PLOGI<<"Resetting Record Directive values";
    javaNativeInterface->CallStaticVoidMethod(klass, resetDirectivesMtdId);
}
