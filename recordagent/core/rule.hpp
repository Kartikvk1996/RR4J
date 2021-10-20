// Header imports
#include "global_includes.hpp"

#ifndef __METHOD__HPP
#define __METHOD__HPP
#include "method.hpp"
#endif // __METHOD__HPP


/**

Rule class defines the way the JVM execution are recorded.

**/

extern std::mutex threadsMonitoredCountLock;
extern std::mutex recordCompleteLock;
extern std::mutex tracingCompleteLock;
extern std::mutex transformationCompleteLock;

class Rule
{
private:
    std::string methodPoint;
    std::string catchException;
    int depth;
    volatile int currentDepth = 0;
    bool additivity = false;
    bool exitOnException = true;
    int maxThreadsToMonitor = 1;
    int threadsMonitoredSoFar = 0;
    std::vector<std::string> threadNames;
    std::shared_ptr<Method> method = NULL;
    bool tracingCompleted = false;
    bool transformationDone = false;
    std::unordered_map<jmethodID, int> tracedMethods;
    std::unordered_map<std::string, jmethodID> methodNameIdMap;
    int history = 0;
    int threadsToRecord = 0;
    volatile bool isMtdHooksAttached = false;

    jclass classTransformer;
    jmethodID transformClassMtdId;
    jmethodID getMtdNameToRestoreMtdId;
    jmethodID getByteCodeToRestoreMtdId;

public:
    Rule(std::string mtd, std::string cException, bool recursive, bool exitOnExp, int maxThreadsMonitorCount, int dpt, int hstry, int tdsToRecord)
    {
       methodPoint = mtd;
       catchException = cException;
       additivity = recursive;
       exitOnException = exitOnExp;
       maxThreadsToMonitor = maxThreadsMonitorCount;
       depth = dpt;
       history = hstry;
       threadsToRecord = tdsToRecord;
    }

    ~Rule()
    {
        cleanup();
        threadNames.clear();
        method.reset();
        tracedMethods.clear();
        methodNameIdMap.clear();
    }

    bool init();
    std::string getMethodPoint();
    std::string getCatchException();
    bool isExceptionSet();
    bool isAdditivity();
    bool isExitOnException();
    int getThreadsToMonitor();
    int getDepth();
    int getCurrentDepth();

    int incrementCurrentDepth();
    int decrementCurrentDepth();
    bool incrementThreadsToMonitor();
    bool decrementThreadsToMonitor();
    bool isThreadsMonitorMaxedOut();

    void setMethod(std::shared_ptr<Method> m);
    std::shared_ptr<Method> getMethod();
    bool isTracingCompleted();
    bool setTracingCompleted();
    bool isTransformationDone();
    bool setTransformationDone();
    void initiateDumpAndWait();
    void start();
    void identifyMethodCalls();
    void startTransformation();
    void startDeTransformation();
    void cleanup();

    bool isMethodHooksAttached();
    bool setMethodHooksAttached();
    int getHistory();
    int getThreadsToRecord();
    bool isPackageBlacklisted(char* klassSignature);

    std::unordered_map<jmethodID, int>& getTracedMethodsList();
    std::unordered_map<std::string, jmethodID>& getMethodNameIdMap();
};
