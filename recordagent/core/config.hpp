#pragma once
#include "global_includes.hpp"

#ifndef __RULE__HPP
#define __RULE__HPP
#include "rule.hpp"
#endif // __RULE__HPP

class Configure
{
private:
    std::list<Rule*> rules;
    Rule* activeRule = NULL;
    std::string logFileName = "agentLog.log";
    int logFileMaxSize = 1024*1024*10; // 10 MB
    int maxLogFiles = 5;
    int delayedStart = 0;
    std::string recordDumpLocation;
    std::string configJSON;
    std::vector<std::regex> blacklistPackages;
    bool serializeThisObj;

public:
    Configure(std::string config)
    {
        configJSON = config;
    }

    ~Configure()
    {
        for(Rule *rule : rules)
        {
            if(rule != NULL)
                delete rule;
        }
        blacklistPackages.clear();
    }

    bool configureAgent();

    std::list<Rule*> getRules();
    Rule* getActiveRule();
    void setActiveRule(Rule* rule);
    std::string getRecordDumpLocation();
    int getDelayedStart();
    std::vector<std::regex>& getBlacklistPackages();
    bool isSerializeThisObj();
};
