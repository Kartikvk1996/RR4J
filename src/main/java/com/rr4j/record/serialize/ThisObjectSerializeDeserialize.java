package com.rr4j.record.serialize;

import io.protostuff.GraphIOUtil;
import io.protostuff.LinkedBuffer;
import io.protostuff.Schema;
import io.protostuff.runtime.RuntimeSchema;

/**
 * <h1>ThisObjectSerializeDeserialize</h1>
 * 
 * Utility to serialize and deserialize the {@link ThisObject}
 * The default buffer size is 65MB which is the object heap size, if this exceeds,
 * user can configure this value by setting -Drecord.serialize.buffersize=&lt;value&gt;
 * 
 * @author kartik
 *
 */
public class ThisObjectSerializeDeserialize 
{
	// Buffer.
	private static int thisObjBufferSize;
	
	static
	{
		thisObjBufferSize = Integer.valueOf(System.getProperty("record.serialize.buffersize", "8192"));
	}

	// Reused buffer.
	LinkedBuffer thisObjBuffer = LinkedBuffer.allocate(thisObjBufferSize); // 65MB ;
	
	// Schema constructed using {@link ThisObject} 
	Schema<ThisObject> thisObjectSchema = RuntimeSchema.getSchema(ThisObject.class);
	
	/**
	 * Serializes the instance of {@link ThisObject}
	 * @param obj instance of {@link ThisObject}
	 * @return byte array if serialization is successful, else empty array if there is an exception.
	 */
	public byte[] serializeObj(ThisObject obj)
	{
		try
		{
			byte[] data = GraphIOUtil.toByteArray(obj, thisObjectSchema, thisObjBuffer);
			return data;
		}
		catch(Exception ex)
		{
		}
		finally
		{
			thisObjBuffer.clear();
		}
		return null;
	}
	
	/**
	 * Deserializes byte array data back to {@link ThisObject}
	 * @param data serialized input data.
	 * @return Instance of {@link ThisObject}
	 */
	public ThisObject deSerializeObj(byte[] data)
	{
		ThisObject deSerObj = thisObjectSchema.newMessage();
		GraphIOUtil.mergeFrom(data, deSerObj, thisObjectSchema);
		return deSerObj;
	}
	
}
