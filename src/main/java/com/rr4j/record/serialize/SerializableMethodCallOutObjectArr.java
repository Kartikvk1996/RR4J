package com.rr4j.record.serialize;

import java.util.ArrayList;
import java.util.List;

import com.rr4j.template.MethodCallOutObjectArr;

/**
 * <h1>SerializableMethodCallOutObjectArr</h1>
 * 
 * Serializable form of {@link MethodCallOutObjectArr}
 *  
 * @author kartik
 *
 */
public class SerializableMethodCallOutObjectArr
{
	// Method return value
	private Object[] methodReturnValue;
	
	// Method return type.
	private List<String> methodReturnTypes;
	
	// Method execution depth
	private int methodDepth;
	
	private String methodName;
	
	// Method execution time.
	private long time;
	
	private boolean isReturnTypeVoid = false;
	
	private String methodClassName;
	
	// Exception object if method has any exception thrown.
	private Exception exception;
	private String exceptionMethodName;
	private String exceptionClassName;
	
	/**
	 * Construct the serializable form of MethodCallOutObjArr from {@link MethodCallOutObjectArr}
	 * @param callOutMethod
	 */
	public void construct(MethodCallOutObjectArr callOutMethod)
	{
		methodDepth = callOutMethod.getMethodDepth();
		time = callOutMethod.getTime();
		methodName = callOutMethod.getMethodName();
		methodClassName = callOutMethod.getMethodClassName();
		Object[] rValues = callOutMethod.getMethodReturnValue();
		exception = callOutMethod.getException();
		exceptionMethodName = callOutMethod.getExceptionMethodName();
		exceptionClassName = callOutMethod.getMethodClassName();
		if(rValues.length == 0)
		{
			isReturnTypeVoid = true;
		}
		else
		{
			methodReturnTypes = new ArrayList<String>();
			methodReturnValue = rValues;
			for(Object o : rValues)
			{
				methodReturnTypes.add(o.getClass().getName());
			}
		}
	}

	/**
	 * @return method return value
	 */
	public Object[] getMethodReturnValue() 
	{
		return methodReturnValue;
	}

	/**
	 * @return method return type
	 */
	public List<String> getMethodReturnTypes() 
	{
		return methodReturnTypes;
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
	 * @return method execution time
	 */
	public long getTime() 
	{
		return time;
	}

	/**
	 * @return true if method returns no value, else false.
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
