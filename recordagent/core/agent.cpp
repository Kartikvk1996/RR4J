#ifndef __AGENT__HPP
#define __AGENT__HPP
#include "agent.hpp"
#endif // __AGENT_HPP

#ifndef __RECORD_DIRECTIVE__HPP
#define __RECORD_DIRECTIVE__HPP
#include "recorddirective.hpp"
#endif // __RECORD_DIRECTIVE__HPP

#ifndef __EXCEPTION__HPP
#define __EXCEPTION__HPP
#include "exception.hpp"
#endif // __EXCEPTION__HPP

#ifndef __SERIALIZER__HPP
#define __SERIALIZER__HPP
#include "serializer.hpp"
#endif // __SERIALIZER__HPP

bool Agent::init()
{
    try
    {
        std::ifstream file("config.json");
        std::string fileData;
        if(file) {
          std::ostringstream ss;
          ss << file.rdbuf(); // reading data
          fileData = ss.str();
        }

        configuration = std::make_shared<Configure>(fileData);
        if(!configuration->configureAgent())
        {
            return false;
        }
    }
    catch(std::exception& e)
    {
        std::cout<<" Failed to initialize agent. Error "<<e.what()<<"\n";
        return false;
    }

    return true;
}

static void JNICALL _startOnThread(jvmtiEnv* jvmti, JNIEnv* jni, void *p)
{
    jvmEnvironment = jvmti;
    javaNativeInterface = jni;
    gblAgent->startOnThread();
}

void Agent::startOnThread()
{
    int dStart = configuration->getDelayedStart();
    if(dStart >= 0)
    {
        int temp = (int)(dStart/1000);
        PLOGI<<"Delayed start is configured, Agent will start in "<<temp<<" seconds";
        while(temp > 0)
        {
            temp-=5;
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            PLOGI<<"Agent will start in "<<temp<<" seconds";
        }
    }

    jclass *cptr;
    jint clsCount;
    jvmtiError er = jvmEnvironment->GetLoadedClasses(&clsCount, &cptr);
    if(Agent::checkErrorAndLog(er) == true)
    {
        PLOGI<<"Failed to index classes for performance";
        return;
    }

    for(int i=0; i<clsCount; i++)
    {
        char* sigPtr;
        char* gPtr;
        er = jvmEnvironment->GetClassSignature(cptr[i], &sigPtr, &gPtr);
        if(Agent::checkErrorAndLog(er) == false)
        {
            std::string cName = sigPtr;
            cName = cName.substr(1, cName.length()-2);
            if(cName.length() !=0 )
            {
                gblClassMap.insert(std::make_pair(cName, cptr[i]));
            }
            Agent::memDeAllocate(sigPtr);
            Agent::memDeAllocate(gPtr);
        }
    }
    Agent::memDeAllocate(cptr);
    PLOGI<<"Indexed "<<clsCount<<" classes";

    gblRecordDirective = new RecordDirective();
    if(gblRecordDirective->init() == false)
    {
        PLOGE<<"Failed to initialize RecordDirectives";
        PLOGI<<"Destroying Agent and shutting down.";
        delete gblRecordDirective;
        delete gblAgent;
        return;
    }

    gblException = new Exception();
    if(gblException->init() == false)
    {
        PLOGE<<"Failed to initialize Exception handler";
        PLOGI<<"Destroying Agent and shutting down.";
        delete gblRecordDirective;
        delete gblException;
        delete gblAgent;
        return;
    }

    gblSerializer = new Serializer();
    if(gblSerializer->init() == false)
    {
        PLOGE<<"Failed to initialize Serializer";
        PLOGI<<"Destroying Agent and shutting down.";
        delete gblRecordDirective;
        delete gblException;
        delete gblAgent;
        delete gblSerializer;
        return;
    }


    std::list<Rule*> rules = configuration->getRules();
    for(Rule* rule : rules)
    {
        try
        {
            if(rule->init() == false)
            {
                PLOGE<<"Failed to initialize Class Transformer";
                return;
            }

            gblRecordDirective->resetDirectives();
            jstring exptn = javaNativeInterface->NewStringUTF(rule->getCatchException().c_str());
            gblRecordDirective->setDirectives(rule->isExitOnException(), rule->getHistory(), rule->getThreadsToRecord(), true, exptn, configuration->isSerializeThisObj());
            configuration->setActiveRule(rule);
            gblRecordDirective->makeRuleActive(true);

            std::shared_ptr<Method> m = std::make_shared<Method>();
            rule->setMethod(m);
            rule->start();
            configuration->setActiveRule(NULL);
            gblRecordDirective->resetDirectives();

            if(rule->isExceptionSet())
            {
                jvmtiError error = jvmEnvironment->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_EXCEPTION, NULL);
                Agent::checkErrorAndLog(error);
            }
        }
        catch(std::exception& e)
        {
            PLOGE <<"Error occurred while starting rule "<<e.what();
        }
    }

    PLOGI<<"Destroying Agent and shutting down.";
    delete gblAgent;
    delete gblRecordDirective;
    return;
}

bool Agent::start()
{
    jclass thrClass;
	jmethodID cid;
	jthread res;

	thrClass = javaNativeInterface->FindClass("java/lang/Thread");
	if (thrClass == NULL)
    {
        PLOGE<<"Failed to find 'java/lang/Thread' class";
        return false;
	}
	cid = javaNativeInterface->GetMethodID(thrClass, "<init>", "()V");
	if (cid == NULL)
    {
        PLOGE<<"Failed to find 'java/lang/Thread' constructor";
        return false;
	}
	res = javaNativeInterface->NewObject(thrClass, cid);
	if (res == NULL)
	{
		PLOGE<<"Failed to create 'java/lang/Thread' object";
        return false;
	}

    jvmtiError error = jvmEnvironment->RunAgentThread(res, &_startOnThread, NULL, JVMTI_THREAD_NORM_PRIORITY);
    if(!checkErrorAndLog(error))
    {
        PLOGI<<"Created new thread and execution is started on that thread";
        return true;
    }
    return false;
}

void Agent::memAllocate(jlong size, unsigned char* ptr)
{
    ptr = NULL;
    jvmtiError error = jvmEnvironment->Allocate(size, &ptr);
    checkErrorAndLog(error);
}

void Agent::memDeAllocate(void *ptr)
{
    jvmtiError error = jvmEnvironment->Deallocate((unsigned char *)ptr);
    checkErrorAndLog(error);
}

bool Agent::checkErrorAndLog(jvmtiError error)
{
    if(error != JVMTI_ERROR_NONE)
    {
        char *errorName = NULL;
        jvmEnvironment->GetErrorName(error, &errorName);
        if(errorName != NULL)
        {
            PLOGE<< "JVM error, errorCode : "<<error<< " errorName : "<< errorName;
            jvmEnvironment->Deallocate((unsigned char *)errorName);
        }
        return true;
    }
    return false;
}

std::shared_ptr<Configure> Agent::getCongiguration()
{
    return configuration;
}



// TODO handle VM death caused by application....
