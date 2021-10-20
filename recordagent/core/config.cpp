#ifndef __CONFIG__HPP
#define __CONFIG__HPP
#include "config.hpp"
#endif // __CONFIG__HPP

bool Configure::configureAgent()
{
    try
    {
        nlohmann::json jObj = nlohmann::json::parse(configJSON);

        logFileName = jObj["logFileName"].get<std::string>();
        logFileMaxSize = jObj["logFileMaxSize"].get<int>();
        maxLogFiles = jObj["maxLogFiles"].get<int>();
        delayedStart = jObj["delayedStart"].get<int>();
        recordDumpLocation = jObj["recordDumpLocation"];
        serializeThisObj = jObj["recordCallerSnapshot"];

        if(recordDumpLocation.length() == 0)
        {
            recordDumpLocation=".";
        }
        else
        {
            if(recordDumpLocation.back() == '/')
                recordDumpLocation.pop_back();
        }

        plog::init(plog::debug, logFileName.c_str(), logFileMaxSize, maxLogFiles);
        PLOGI << "Initialized the logger. FileName = "<<logFileName<<" MaxSize = "<<logFileMaxSize<<" MaxLogFiles = "<<maxLogFiles;

        for (const auto& jRule : jObj["rules"])
        {
            Rule *r = new Rule(jRule["method"],jRule["exception"],jRule["additivity"].get<bool>(),jRule["exitOnException"].get<bool>(), jRule["threadsToMonitor"].get<int>(),
                               jRule["depth"].get<int>(), jRule["history"].get<int>(), jRule["threadsToRecord"].get<int>());

            PLOGI << "Adding rule : "<<jRule.dump();

            rules.push_back(r);
        }


        for(const auto blp : jObj["blacklistPackages"])
        {
            PLOGI<<"Blacklisting package : "<<blp;
            std::string s= "(.*)("; s.append(blp); s.append(")(.*)");
            blacklistPackages.push_back(std::regex(s));
        }

        return true;
    }
    catch(std::exception& e)
    {
        std::cout<<"Failed to configure the agent. Error = "<<e.what()<<"\n";
        return false;
    }
}

std::list<Rule*> Configure::getRules()
{
    return rules;
}

Rule* Configure::getActiveRule()
{
    return activeRule;
}

void Configure::setActiveRule(Rule* rule)
{
    activeRule = rule;
}

std::string Configure::getRecordDumpLocation()
{
    return recordDumpLocation;
}

int Configure::getDelayedStart()
{
    return delayedStart;
}

std::vector<std::regex>& Configure::getBlacklistPackages()
{
    return blacklistPackages;
}

bool Configure::isSerializeThisObj()
{
    return serializeThisObj;
}
