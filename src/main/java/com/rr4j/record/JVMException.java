package com.rr4j.record;

/**
 * <h1>JVMException</h1>
 * 
 * It is an exception handler. when an exception is thrown by a particular function,
 * 'exceptionCallback' function is called.
 * 
 * @author kartik
 */
public class JVMException 
{	

	/**
	 * Callback called from attached agent.
	 * @param thread	Name of the thread.
	 * @param exception	Exception Object
	 * @param className	Name of the class.
	 * @param methodName	Method name.
	 * @return	true if exception is handled, else false is retrned.
	 */
	public static boolean exceptionCallback(Thread thread, Object exception, String className, String methodName)
	{
		Exception ex = (Exception) exception;
		if(RecordDirectives.IS_ACTIVE_RULE_SET && RecordDirectives.RECORDING_STATUS == 1)
		{
			if(ex.toString().contains(RecordDirectives.WATCH_EXCEPTION))
			{
				RecordDirectives.IS_EXCEPTION_CAUGHT = true;
				Record.handleException(thread, ex, className, methodName);
				
				if(RecordDirectives.EXIT_ON_EXCEPTION == true)
				{
					RecordDirectives.RECORDING_STATUS = 2;
				}
				return true;
			}
		}
		return false;
	}
}
