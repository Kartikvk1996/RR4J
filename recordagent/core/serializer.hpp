// Header imports
#include "global_includes.hpp"

/**
    Bridge to interact with java "Serializer class"
**/
class Serializer
{
    jclass klass;
    jmethodID serializeDataMtdId;

public:

    /**
        Initializes by identifying whether class if found in JVM env.
    **/
    bool init();

    /**
        Serializes the recorded data by calling Java layer.
    **/
    bool serializeData(std::string recordDumpPath);
};

extern Serializer *gblSerializer;
