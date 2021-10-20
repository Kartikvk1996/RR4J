package com.rr4j.record;

/**
 * <h1>RecordDirectives</h1>
 * 
 * Configured by the attached agent based on provided input rule.
 * 
 * @author kartik
 */
public class RecordDirectives 
{
	public static boolean EXIT_ON_EXCEPTION = false;
	
	public static boolean IS_EXCEPTION_CAUGHT = false;
	
	public static int REMEMBER_HISTORY = 0;
	
	public static int RECORDING_STATUS = 0;
	
	public static int THREADS_TO_RECORD = 0;
	
	public static boolean IS_ACTIVE_RULE_SET = false;
	
	public static boolean INITIATE_DUMP = false;
	
	public static boolean IS_DUMP_COMPLETED = false;
	
	public static String WATCH_EXCEPTION = null;
	
	public static boolean SERIALIZE_THISOBJECT = true;
	
	/**
	 * Sets record directives, based on this value record flow is decided.
	 * @param exitOnException	Stop recording once the exception is caught.
	 * @param rememberHistory	Execution history to remember.
	 * @param threadsToRecord	Total threads to record.
	 * @param activeRuleSet		Is active rule set.
	 * @param watchException	Should lookout for any exception.
	 * @param serializeThisObj	Should record calling object parameters, useful if instance values are modified by function.
	 */
	public static void setDirectives(boolean exitOnException, int rememberHistory, int threadsToRecord, boolean activeRuleSet, String watchException, boolean serializeThisObj)
	{
		EXIT_ON_EXCEPTION = exitOnException;
		REMEMBER_HISTORY = rememberHistory;
		THREADS_TO_RECORD = threadsToRecord;
		IS_ACTIVE_RULE_SET = activeRuleSet;
		WATCH_EXCEPTION = watchException;
		SERIALIZE_THISOBJECT = serializeThisObj;
	}
	
	/**
	 * Set exit on exception.
	 * @param value
	 */
	public static void setExitOnException(boolean value)
	{
		EXIT_ON_EXCEPTION = value;
	}
	
	/**
	 * Set exception caught.
	 * @param value
	 */
	public static void setExceptionCaught(boolean value)
	{
		IS_EXCEPTION_CAUGHT = true;
	}
	
	/**
	 * Total history to remember.
	 * @param value
	 */
	public static void setRememberHistory(int value)
	{
		REMEMBER_HISTORY = value;
	}
	
	/**
	 * Total threads to record.
	 * @param value
	 */
	public static void setThreadsToRecord(int value)
	{
		THREADS_TO_RECORD = value;
	}
	
	/**
	 * Set rule to active state.
	 * @param value
	 */
	public static void makeRuleActive(boolean value)
	{
		IS_ACTIVE_RULE_SET = value;
	}
	
	/**
	 * @return Whether record dumping process is completed or not.
	 */
	public static boolean isDumpCompleted()
	{
		return IS_DUMP_COMPLETED;
	}
	
	/**
	 * Starts record dumping process.
	 */
	public static void initiateDumping()
	{
		INITIATE_DUMP = true;
	}
	
	/**
	 * Sets recording status.
	 * <ul>
	 * 	<li>0 = Recording not started</li>
	 *	<li>1 = Recording started</li>
	 * 	<li>2 = Recording completed</li>
	 * </ul>
	 * @param value
	 */
	public static void setRecordingStatus(int value)
	{
		RECORDING_STATUS = value;
	}
	
	/**
	 * @return Recording status
	 */
	public static int getRecordingStatus()
	{
		return RECORDING_STATUS;
	}
	
	/**
	 * Resets record directives.
	 */
	public static void resetDirectives()
	{
		EXIT_ON_EXCEPTION = false;
		IS_EXCEPTION_CAUGHT = false;
		REMEMBER_HISTORY = 0;
		THREADS_TO_RECORD = 0;
		IS_ACTIVE_RULE_SET = false;
		INITIATE_DUMP = false;
		IS_DUMP_COMPLETED = false;
		WATCH_EXCEPTION = null;
		Record.reset();
	}
}
