package com.rr4j.replay.utils;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.rr4j.record.utils.NativeTypeMapper;
import com.rr4j.record.utils.TypeMapper;

/**
 * <h1>MethodParser</h1>
 * 
 * Parses the method and extracts argument types and return type.
 * 
 * 
 * 
 * @author kartik
 *
 */
public class MethodParser 
{
	private static final Logger logger = LogManager.getLogger(MethodParser.class);
	private static final Pattern pattern= Pattern.compile("\\((.*?)\\)");
	private StringBuilder buffer = new StringBuilder();
	private String method;
	private String methodArgs;
	private String methodReturnTypeNative;
	private List<Class<?>> methodArgsTypesList = null;
	private Class<?>[] methodArgsTypeArray = null;
	private Class<?> methodReturnType = null;
	private String bareMethodName;
	
	public MethodParser(String method) 
	{
		this.method = method;
	}
	
	public MethodParser() 
	{
	
	}
	
	/**
	 * Parses the method and extracts argument types and return type.
	 */
	public void parse()
	{
		logger.info("Method '{}' will be parsed for arguments", method);
		
		bareMethodName = method.substring(0, method.indexOf(TypeMapper.OPENING_BRACKET)).trim();
		methodReturnTypeNative = method.substring(method.indexOf(TypeMapper.CLOSING_BRACKET)+1, method.length());
		Matcher matcher = pattern.matcher(method);
		if(matcher.find())
		{
			methodArgs = matcher.group(1);
		}
		
		logger.info("Extracted arguments : {}", methodArgs);
		logger.info("Extracted return type : {}", methodReturnTypeNative);
		if(methodArgs.length() == 0)
		{
			if(methodReturnTypeNative.length() > 0)
				extractReturnType();
			methodArgsTypesList = Collections.emptyList();
		}
		else
		{
			extractArgs();
			if(methodReturnTypeNative.length() > 0)
				extractReturnType();
			
			if(methodArgsTypesList.size()==0)
				methodArgsTypeArray = new Class<?>[0];
			else
			{
				methodArgsTypeArray = new Class<?>[methodArgsTypesList.size()];
				int cntr = 0;
				for(Class<?> klass : methodArgsTypesList)
					methodArgsTypeArray[cntr++] = klass;
			}
		}
	}
	
	/**
	 * Example if method is 'testMethod(Ljava/lang/String;C)Z'
	 * then list contains [String.class, char.class] 
	 * 
	 * @return List of argument class types.
	 */
	public List<Class<?>> getMethodArgsTypesList() 
	{
		return methodArgsTypesList;
	}

	/**
	 * @return Array of argument class types.
	 */
	public Class<?>[] getMethodArgsTypeArray() 
	{
		return methodArgsTypeArray;
	}
	
	/**
	 * @return Method name
	 */
	public String getMethod() 
	{
		return method;
	}

	/**
	 * Sets method name.
	 * @param method
	 */
	public void setMethod(String method)
	{
		this.method = method;
	}
	
	/**
	 * Example if method is 'testMethod(Ljava/lang/String;C)Z'
	 * then 'testMethod' is returned.  
	 * 
	 * @return bare method name. 
	 */
	public String getBareMethodName() 
	{
		return bareMethodName;
	}

	/**
	 * Example if method is 'testMethod(Ljava/lang/String;C)Z'
	 * then 'Boolean.class' is returned.
	 * 
	 * @return method return type
	 */
	public Class<?> getMethodReturnType() 
	{
		return methodReturnType;
	}

	/**
	 * Extracts arguments from method 
	 */
	private void extractArgs()
	{
		methodArgsTypesList = new ArrayList<Class<?>>();
		buffer.delete(0, buffer.length());
		int index = 0;
		int maxIndex = methodArgs.length();
		char argsCharArray[] = methodArgs.toCharArray();
		
		
		while(index < maxIndex)
		{
			buffer.delete(0, buffer.length());
			int arrayTypeCount = 0;
			boolean isNativeType = false;
			char currentChar = argsCharArray[index];
			String mappedValue = TypeMapper.mapper.get(currentChar);

			if(mappedValue == null) // might be either fully qualified name or array
			{
				if(currentChar == 'L')
				{
					index++;
					// run till we see semicolon to extract arg
					while(argsCharArray[index] != TypeMapper.SEMICOLON)
					{
						if(argsCharArray[index] == '/')
							argsCharArray[index] = '.';
						
						buffer.append(argsCharArray[index]);
						index++;
					}
				}
				else if(currentChar == TypeMapper.OPENING_SQUARE_BRACKET) // array
				{
					while(argsCharArray[index] != TypeMapper.OPENING_SQUARE_BRACKET)
					{
						index++;
						arrayTypeCount++;
					}
					
					if(TypeMapper.mapper.get(argsCharArray[index]) == null) // fully qualified array
					{
						index++;
						while(argsCharArray[index] != TypeMapper.SEMICOLON)
						{
							if(argsCharArray[index] == '/')
								argsCharArray[index] = '.';
							
							buffer.append(argsCharArray[index]);
							index++;
						}
					}
					else
					{
						buffer.append(TypeMapper.mapper.get(argsCharArray[index]));
						isNativeType = true;
					}
				}
			}
			else
			{
				buffer.append(mappedValue);
				isNativeType = true;
			}
			
			methodArgsTypesList.add(getClassType(buffer.toString(), arrayTypeCount, isNativeType));
			
			index++;
		}
	}
	
	/**
	 * Extracts return type from method
	 */
	private void extractReturnType()
	{
		buffer.delete(0, buffer.length());
		char rTypeCharArray[] = methodReturnTypeNative.toCharArray();
		int index = 0;
		char currentChar = rTypeCharArray[index];
		String mappedValue = TypeMapper.mapper.get(currentChar);
		
		int arrayTypeCount = 0;
		boolean isNativeType = false;
		
		if(mappedValue == null) // might be either fully qualified name or array
		{
			if(currentChar == 'L')
			{
				index++;
				// run till we see semicolon to extract arg
				while(rTypeCharArray[index] != TypeMapper.SEMICOLON)
				{
					if(rTypeCharArray[index] == '/')
						rTypeCharArray[index] = '.';
					
					buffer.append(rTypeCharArray[index]);
					index++;
				}
			}
			else if(currentChar == TypeMapper.OPENING_SQUARE_BRACKET) // array
			{
				while(rTypeCharArray[index] == TypeMapper.OPENING_SQUARE_BRACKET)
				{
					index++;
					arrayTypeCount++;
				}
				
				if(TypeMapper.mapper.get(rTypeCharArray[index]) == null) // fully qualified array
				{
					index++;
					while(rTypeCharArray[index] != TypeMapper.SEMICOLON)
					{
						if(rTypeCharArray[index] == '/')
							rTypeCharArray[index] = '.';
						
						buffer.append(rTypeCharArray[index]);
						index++;
					}
				}
				else
				{
					buffer.append(TypeMapper.mapper.get(rTypeCharArray[index]));
					isNativeType = true;
				}
			}
		}
		else
		{
			buffer.append(mappedValue);
			isNativeType = true;
		}
		
		methodReturnType = getClassType(buffer.toString(), arrayTypeCount, isNativeType);
	}
	
	private Class<?> getClassType(String className, int arrayCount, boolean isNativeType)
	{
		Class<?> klass = null;
		try
		{
			Class<?> temp = null;
			if(isNativeType)
			{
				temp = NativeTypeMapper.mapper.get(className);
			}
			else
			{
				temp = Class.forName(className);
			}
			
			while(arrayCount > 0)
			{
				arrayCount--;
				temp = Array.newInstance(temp, 0).getClass();
			}
			klass = temp;
		}
		catch(ClassNotFoundException ex)
		{
			logger.error(ex);
		}
		return klass;
	}
}
