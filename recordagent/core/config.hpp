#pragma once
#include "global_includes.hpp"

#ifndef __RULE__HPP
#define __RULE__HPP
#include "rule.hpp"
#endif // __RULE__HPP

/**
    Class that holds agent configuration information.
**/
class Configure
{
private:
    // Holds list of rules defined.
    std::list<Rule*> rules;

    // Current active rule.
    Rule* activeRule = NULL;

    // Log file name.
    std::string logFileName = "agentLog.log";

    // Log file size in bytes.
    int logFileMaxSize = 1024*1024*10; // 10 MB

    // Max log files (rotation is enabled)
    int maxLogFiles = 5;

    // Delayed start in bytes.
    int delayedStart = 0;

    // Record dump location
    std::string recordDumpLocation;

    // Configuration JSON content.
    std::string configJSON;

    // Blacklisted packages.
    std::vector<std::regex> blacklistPackages;

    // Serialize the caller object or not.
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

    /**
        Reads the configuration file and configures the agent.
    **/
    bool configureAgent();

    /**
        Returns list of all rules.
    **/
    std::list<Rule*> getRules();

    /**
        Returns current active rule.
    **/
    Rule* getActiveRule();

    /**
        Sets current active rule.
    **/
    void setActiveRule(Rule* rule);

    /**
        Returns path where the record dump is created.
    **/
    std::string getRecordDumpLocation();

    /**
        Returns value in milliseconds to delay the startup of an agent.
    **/
    int getDelayedStart();

    /**
        Returns regex list of all blacklisted packages.
    **/
    std::vector<std::regex>& getBlacklistPackages();

    /**
        Returns true if java 'this' object to serialized, useful when instance variables are changed for each iteration and
        you need to retain the data.
    **/
    bool isSerializeThisObj();
};
