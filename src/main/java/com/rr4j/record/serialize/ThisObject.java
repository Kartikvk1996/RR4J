package com.rr4j.record.serialize;

/**
 * <h1>ThisObject</h1>
 * 
 * Wrapper to serialize the recorded object.
 * Since we are using protobuff serializer, it needs schema to serialize.
 * as the schema and class name are developer decided, we cannot create schemas for
 * all the classes, hence we create a wrapper and protobuff recursively construct the schema
 * at runtime.
 * 
 * @author kartik
 */
public class ThisObject
{
	private Object object;
	
	public void setObject(Object object) 
	{
		this.object = object;
	}

	public Object getObject() 
	{
		return object;
	}
}
