#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

#ifndef __SERIALIZER__HPP
#define __SERIALIZER__HPP
#include "serializer.hpp"
#endif // __SERIALIZER__HPP



bool Serializer::init()
{
    klass = javaNativeInterface->FindClass("com/recordreplay/record/serialize/Serializer");
    if(klass == NULL)
    {
        PLOGE<<"Failed to find class 'com/recordreplay/record/serialize/Serializer'";
        return false;
    }

    serializeDataMtdId = javaNativeInterface->GetStaticMethodID(klass, "serializeData", "(Ljava/lang/String;)Z");
    if(serializeDataMtdId == NULL)
    {
        PLOGE<<"Failed to find method 'com.recordreplay.record.Serializer::serializeData'";
        return false;
    }
    return true;
}

bool Serializer::serializeData(std::string recordDumpPath)
{
    PLOGI<<"Staring to serialize the recorded data";
    bool status = javaNativeInterface->CallStaticBooleanMethod(klass, serializeDataMtdId, javaNativeInterface->NewStringUTF(recordDumpPath.c_str()));
    PLOGI<<"Serialization completed";
    return status;
}
