// Header imports
#include "global_includes.hpp"

// Bridge to interact with java "Serializer class"

class Serializer
{
    jclass klass;
    jmethodID serializeDataMtdId;

public:
    bool init();
    bool serializeData(std::string recordDumpPath);
};

extern Serializer *gblSerializer;
