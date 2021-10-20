package com.rr4j.template;

/**
 * <h1>MethodCallIn</h1>
 * 
 * A basic information of method call is stored.
 * 
 * @author kartik
 *
 */
public class MethodCallIn
{
	private Object[] methodArguments = null;
	private final int methodDepth;
	private final String methodName;
	private final long time;
	private final String methodClassName;

	public MethodCallIn(Object[] inputArgs, int depth, String mName, String mClassName)
	{
		time = System.currentTimeMillis();
		methodArguments = inputArgs;
		methodDepth = depth;
		methodName = mName;
		methodClassName = mClassName;
	}

	public Object[] getMethodArguments() 
	{
		return methodArguments;
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
	
	
}
