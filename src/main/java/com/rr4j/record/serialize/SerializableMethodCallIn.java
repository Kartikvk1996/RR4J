package com.rr4j.record.serialize;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.rr4j.template.MethodCallIn;

/**
 * <h1>SerializableMethodCallIn</h1>
 * 
 * Serializable form of {@link MethodCallIn}
 * 
 * @author kartik
 *
 */
public class SerializableMethodCallIn implements Serializable
{
	private static final long serialVersionUID = 1L;
	
	// method depth at which it is executed.
	private int methodDepth;
	private String methodName;
	
	// Time at which method is executed.
	private long time;
	
	// Input arguments types.
	private List<String> argsType = null;
	private boolean emptyArgs = false;
	
	// Input arguments passed to method.
	private Object[] inputArgs = null;
	
	private String methodClassName;
	
	/**
	 * Constructs the serializable for of MethodCallIn from provided input instance.
	 * @param callInMethod
	 */
	public void construct(MethodCallIn callInMethod)
	{
		methodDepth = callInMethod.getMethodDepth();
		methodName = callInMethod.getMethodName();
		time = callInMethod.getTime();
		methodClassName = callInMethod.getMethodClassName();
		Object[] args = callInMethod.getMethodArguments();
		if(args.length == 0)
		{
			emptyArgs = true;
			argsType = null;
		}
		else
		{
			argsType = new ArrayList<String>();
			for(Object eachArg : args)
			{
				Class<?> kls = eachArg.getClass();
				argsType.add(kls.getTypeName());
			}
			inputArgs = args;
		}
	}

	/**
	 * @return Depth at which method is executed.
	 */
	public int getMethodDepth() 
	{
		return methodDepth;
	}

	/**
	 * @return Method name.
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
	 * @return List of input arguments types.
	 */
	public List<String> getArgsType() 
	{
		return argsType;
	}

	/**
	 * @return true, if method takes no input. else, false is returned.
	 */
	public boolean isEmptyArgs() 
	{
		return emptyArgs;
	}

	/**
	 * @return Object array of method inputs.
	 */
	public Object[] getInputArgs() 
	{
		return inputArgs;
	}

	/**
	 * @return method class name
	 */
	public String getMethodClassName()
	{
		return methodClassName;
	}	
}
