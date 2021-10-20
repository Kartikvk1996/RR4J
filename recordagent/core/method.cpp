#ifndef __METHOD__HPP
#define __METHOD__HPP
#include "method.hpp"
#endif // __METHOD__HPP

#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

bool Method::addHook(std::string name)
{
    jvmtiError error = JVMTI_ERROR_NONE;
    try
    {
        bool isMethodFound = false;
        bool isHookAttached = false;
        std::string className;
        std::string methodName;
        std::string delim = "::";
        size_t position = 0;
        position = name.find(delim);
        if(position != std::string::npos)
        {
            className = name.substr(0, position);
            name.erase(0,position+delim.length());
            methodName = name;
        }

        jclass kls;
        if(gblClassMap.find(className) == gblClassMap.end())
        {
            PLOGI<<"Didn't find class in global cache";
            kls = javaNativeInterface->FindClass(className.c_str());

            if(kls == NULL)
            {
                PLOGE <<"Failed to fetch the class. Name : "<<className;
                return false;
            }
            PLOGI << "Fetched class = '"<<className<<"' from running JVM";
        }
        else
        {
            kls = gblClassMap[className];
            PLOGI<<"Obtained class "<<kls<<" from global cache";
        }



        jint methodCtr;
        jmethodID *methodPtr = NULL;
        error = jvmEnvironment->GetClassMethods(kls, &methodCtr, &methodPtr);
        if(!Agent::checkErrorAndLog(error))
        {
            PLOGI<< "Fetched method list for a given class :"<<className;

            for(jint ittr = 0; ittr < methodCtr; ittr++)
            {
                char* mName;
                char* mSignature;
                char* mGenericPtr;
                error = jvmEnvironment->GetMethodName(methodPtr[ittr], &mName, &mSignature, &mGenericPtr);
                if(!Agent::checkErrorAndLog(error))
                {
                    PLOGI<<"Method : "<<mName<<mSignature<<", Method ID : "<<methodPtr[ittr];

                    if(isMethodFound == false)
                    {
                        std::string obtainedMethodName = std::string(mName);
                        obtainedMethodName+=mSignature;
                        if(obtainedMethodName.compare(methodName) == 0)
                        {
                            isMethodFound = true;
                            PLOGI<<"Found matching method. Provided : "<<methodName<<" Obtained from class file : "<<obtainedMethodName;
                            jvmtiLineNumberEntry *lineNumberEntry = NULL;

                            jint entryCountPtr;
                            error = jvmEnvironment->GetLineNumberTable(methodPtr[ittr], &entryCountPtr, &lineNumberEntry);
                            if(!Agent::checkErrorAndLog(error))
                            {
                                PLOGI<<"Method "<<methodName<<" Start from : "<<lineNumberEntry->start_location<<" Line number : "<<lineNumberEntry->line_number;
                                error = jvmEnvironment->SetBreakpoint(methodPtr[ittr], 0);

                                if(!Agent::checkErrorAndLog(error))
                                {
                                    PLOGI<<"Successfully attached hook to method : "<<methodName<<" Line : "<<lineNumberEntry->line_number<<" MethodId : "<<methodPtr[ittr];
                                    hooks.emplace_back(methodPtr[ittr], 0);
                                    isHookAttached = true;
                                }
                                Agent::memDeAllocate(lineNumberEntry);
                            }
                        }
                    }

                    Agent::memDeAllocate(mName);
                    Agent::memDeAllocate(mSignature);
                    Agent::memDeAllocate(mGenericPtr);
                }
            }
            // Free method array
            Agent::memDeAllocate(methodPtr);

            if(isHookAttached)
                return true;
        }
    }
    catch(std::exception& e)
    {
        PLOGE<<" Exception occurred while attaching method hook "<< e.what();
        return false;
    }
    return false;
}

void Method::clearAllHooks()
{
    for(auto hook : hooks)
    {
        clearHook(hook.first, hook.second);
    }
}

bool Method::clearHook(jmethodID mtd, jlocation loc)
{
    jvmtiError error = jvmEnvironment->ClearBreakpoint(mtd, loc);
    if(!Agent::checkErrorAndLog(error))
    {
        PLOGI<< "Successfully detached method hook. MethodID : " <<mtd << " MethodLocation : " << loc;
        return true;
    }
    PLOGE<< "Failed to detach method hook. MethodID : " <<mtd << " MethodLocation : " << loc;
    return false;
}
