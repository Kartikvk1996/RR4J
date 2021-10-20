/**

Defines all the imports used by the project

**/

#pragma once

// All CPP header files are imported below.
#include <stdlib.h>
#include <list>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <vector>
#include <thread>
#include <unordered_map>
#include <chrono>

// All JVM header files are imported below
#include "../jvm/jvmti.h"
#include "../jvm/jni.h"
#include "../jvm/jvmticmlr.h"
#include "../jvm/jni_md.h"

// Logger
#include "../ext/plog/Log.h"
#include "../ext/plog/Initializers/RollingFileInitializer.h"

// Extra libs
#include "../ext/json/json.hpp"

