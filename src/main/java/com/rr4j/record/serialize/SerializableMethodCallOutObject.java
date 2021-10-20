package com.rr4j.record.serialize;

import java.io.Serializable;

import com.rr4j.template.MethodCallOutObject;

/**
 * <h1>SerializableMethodCallOutObject</h1>
 * 
 * Serializable form of {@link MethodCallOutObject}
 * 
 * @author kartik
 *
 */
public class SerializableMethodCallOutObject implements Serializable 
{
	private static final long serialVersionUID = 1L;
	
	// Method return value.
	private Object methodReturnValue;
	
	// Method return type.
	private String methodReturnType;
	
	// Method execution depth.
	private int methodDepth;

	private String methodName;
	
	// Time at which method is executed.
	private long time;
	
	private boolean isReturnTypeVoid = false;
	
	private String methodClassName;
	
	// Exception object if method has any exception thrown.
	private Exception exception;
	private String exceptionMethodName;
	private String exceptionClassName;
	
	/**
	 * Construct the serializable form of MethodCallOutObj from {@link MethodCallOutObject}
	 * @param callOutMethod
	 */
	public void construct(MethodCallOutObject callOutMethod)
	{
		methodDepth = callOutMethod.getMethodDepth();
		time = callOutMethod.getTime();
		methodName = callOutMethod.getMethodName();
		methodClassName = callOutMethod.getMethodClassName();
		Object rValue = callOutMethod.getMethodReturnValue();
		exception = callOutMethod.getException();
		exceptionMethodName = callOutMethod.getExceptionMethodName();
		exceptionClassName = callOutMethod.getMethodClassName();
		if(rValue == null)
		{
			isReturnTypeVoid = true;
		}
		else
		{
			methodReturnType = rValue.getClass().getTypeName();
			methodReturnValue = rValue;
		}
	}

	/**
	 * @return method return value
	 */
	public Object getMethodReturnValue() 
	{
		return methodReturnValue;
	}

	/**
	 * @return method return type
	 */
	public String getMethodReturnType() 
	{
		return methodReturnType;
	}

	/**
	 * @return method execution depth
	 */
	public int getMethodDepth()
	{
		return methodDepth;
	}

	/**
	 * @return method name
	 */
	public String getMethodName() 
	{
		return methodName;
	}

	/**
	 * @return method execution time.
	 */
	public long getTime() 
	{
		return time;
	}

	/**
	 * @return true if return type is void, else false is returned.
	 */
	public boolean isReturnTypeVoid()
	{
		return isReturnTypeVoid;
	}
	
	/**
	 * @return method class name
	 */
	public String getMethodClassName()
	{
		return methodClassName;
	}

	/**
	 * @return Exception object if exception is caught else, null.
	 */
	public Exception getException()
	{
		return exception;
	}

	/**
	 * @return Method name where exception is generated.
	 */
	public String getExceptionMethodName() 
	{
		return exceptionMethodName;
	}

	/**
	 * @return exception class name.
	 */
	public String getExceptionClassName()
	{
		return exceptionClassName;
	}	
}
