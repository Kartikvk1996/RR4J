// Header imports
#include "global_includes.hpp"

#ifndef __METHOD__HPP
#define __METHOD__HPP
#include "method.hpp"
#endif // __METHOD__HPP

extern std::mutex threadsMonitoredCountLock;
extern std::mutex recordCompleteLock;
extern std::mutex tracingCompleteLock;
extern std::mutex transformationCompleteLock;

/**
    Rule class defines the way the JVM execution are recorded.
**/
class Rule
{
private:
    // Method to interest.
    std::string methodPoint;

    // Exception if specified.
    std::string catchException;

    // Record depth
    int depth;

    volatile int currentDepth = 0;

    bool additivity = false;

    // Stop recording on first matching exception.
    bool exitOnException = true;

    int maxThreadsToMonitor = 1;

    // Thread count who are watching the code flow and finding methods called.
    int threadsMonitoredSoFar = 0;

    // Thread names.
    std::vector<std::string> threadNames;

    // Method information.
    std::shared_ptr<Method> method = NULL;

    bool tracingCompleted = false;
    bool transformationDone = false;

    // All methods identified, which are called from the method point specified in rule.
    std::unordered_map<jmethodID, int> tracedMethods;

    // Map holding method name and method id which is used later for quick lookup.
    std::unordered_map<std::string, jmethodID> methodNameIdMap;

    // Execution history to be recorded.
    int history = 0;

    // Total threads whose execution are recorded.
    int threadsToRecord = 0;

    volatile bool isMtdHooksAttached = false;

    // Used to transform the class bytecode.
    jclass classTransformer;
    jmethodID transformClassMtdId;

    // Used to restore the old behavior once the recording is completed or fatal error occurred.
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

    /**
        Initializes the Rule and returns the status based on success or failure.
        Check all the required class are found and configure the variables.
    **/
    bool init();

    /**
        Returns entry method name.
    **/
    std::string getMethodPoint();

    /**
        Returns exception name.
    **/
    std::string getCatchException();

    /**
        Returns true if exception is set, else false.
    **/
    bool isExceptionSet();

    /**
        TODO: Not implemented.
    **/
    bool isAdditivity();

    /**
        Returns true, if recording must be stopped on encountering first matching exception.
    **/
    bool isExitOnException();
    int getThreadsToMonitor();

    /**
        Returns max depth.
    **/
    int getDepth();

    /**
        Return current depth.
    **/
    int getCurrentDepth();

    /**
        Increments current depth count when method is entered.
    **/
    int incrementCurrentDepth();

    /**
        Decrements current depth count when method is exited.
    **/
    int decrementCurrentDepth();

    bool incrementThreadsToMonitor();
    bool decrementThreadsToMonitor();
    bool isThreadsMonitorMaxedOut();

    /**
        Sets Method object holding method information.
    **/
    void setMethod(std::shared_ptr<Method> m);

    /**
        Returns Method object.
    **/
    std::shared_ptr<Method> getMethod();

    /**
        Returns true, if method tracing is completed.
    **/
    bool isTracingCompleted();

    /**
        Sets tracing completed to true.
    **/
    bool setTracingCompleted();

    /**
        Returns true, if method transformation is completed.
    **/
    bool isTransformationDone();

    /**
        Sets method transformation to true.
    **/
    bool setTransformationDone();

    /**
        Starts the serialization process of all the recorded information.
        This is a blocking call and is only returned once the process is completed or any
        fatal error occurred.
    **/
    void initiateDumpAndWait();

    /**
        Starts the Rule
    **/
    void start();

    /**
        We don't know what all methods will be called. since it is controlled by input data. So we
        keep the first iteration to identify all the methods. Doing this runtime is very useful in-terms of performance.
        else we need to parse the byte code and identify the "invokevirtual" / "invokestatic" calls which is challenging in java
        because of dynamic method call dispatch
    **/
    void identifyMethodCalls();

    /**
        Once we identify the methods, we transform the methods. See Java layer com.rr4j.record.transform.ClassTransform
    **/
    void startTransformation();

    /**
        Restores the old bytecode of all the transformed methods.
    **/
    void startDeTransformation();

    /**
        Does clean up of all pointers.
    **/
    void cleanup();

    /**
        Returns true if method is found in JVM and hooks are attached.
    **/
    bool isMethodHooksAttached();

    /**
        Sets the method hook attached to true.
    **/
    bool setMethodHooksAttached();

    /**
        Returns executing history value set.
    **/
    int getHistory();

    int getThreadsToRecord();

    /**
        Returns true if the particular package is blacklisted which is not transformed.
    **/
    bool isPackageBlacklisted(char* klassSignature);

    /**
        Returns the map of method ids and the depth at which there were encountered.
    **/
    std::unordered_map<jmethodID, int>& getTracedMethodsList();

    /**
        Returns the map of method names and its corresponding method ids.
    **/
    std::unordered_map<std::string, jmethodID>& getMethodNameIdMap();
};
