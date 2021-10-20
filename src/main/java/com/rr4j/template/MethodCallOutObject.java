package com.rr4j.template;

/**
 * <h1>MethodCallOutObject</h1>
 * 
 * A basic information of method return is stored.
 * 
 * @author Kartik
 *
 */
public class MethodCallOutObject
{
	private final Object methodReturnValue;
	private final int methodDepth;
	private final String methodName;
	private final long time;
	private final String methodClassName;
	private Exception exception;
	private String exceptionMethodName;
	private String exceptionClassName;
	
	public MethodCallOutObject(Object returnValue, int depth, String mName, String mClassName) 
	{
		time = System.currentTimeMillis();
		methodReturnValue = returnValue;
		methodDepth = depth;
		methodName = mName;
		methodClassName = mClassName;
	}

	public Object getMethodReturnValue() 
	{
		return methodReturnValue;
	}

	public int getMethodDepth()
	{
		return methodDepth;
	}

	public String getMethodName() 
	{
		return methodName;
	}
	
	public long getTime()
	{
		return time;
	}
	
	public String getMethodClassName() 
	{
		return methodClassName;
	}
	
	public Exception getException()
	{
		return exception;
	}

	public void setException(Exception exception) 
	{
		this.exception = exception;
	}

	public String getExceptionMethodName()
	{
		return exceptionMethodName;
	}

	public void setExceptionMethodName(String exceptionMethodName)
	{
		this.exceptionMethodName = exceptionMethodName;
	}

	public String getExceptionClassName() 
	{
		return exceptionClassName;
	}

	public void setExceptionClassName(String exceptionClassName) 
	{
		this.exceptionClassName = exceptionClassName;
	}
	
	
}
