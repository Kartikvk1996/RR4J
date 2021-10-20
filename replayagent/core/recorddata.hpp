#pragma once
#include "global_includes.hpp"

struct RecordMethodData
{
    jvalue rValue;
    std::string methodName;
    char methodType;
    int depth;
    jmethodID methodId;
};
