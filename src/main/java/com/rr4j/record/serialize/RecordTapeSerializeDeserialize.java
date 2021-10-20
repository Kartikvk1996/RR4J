package com.rr4j.record.serialize;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import io.protostuff.GraphIOUtil;
import io.protostuff.LinkedBuffer;
import io.protostuff.Schema;
import io.protostuff.runtime.RuntimeSchema;

/**
 * <h1>RecordTapeSerializeDeserialize</h1>
 * 
 * Serialize and deserialize the {@link SerializableRecordTape}
 * 
 * @author kartik
 */
public class RecordTapeSerializeDeserialize 
{
	private static final Logger sLogger = LogManager.getLogger(RecordTapeSerializeDeserialize.class);

	private static int SerializerBufferSize;
	
	static
	{
		SerializerBufferSize = Integer.valueOf(System.getProperty("record.serialize.buffersize", "65535"));
	}
	
    LinkedBuffer recordTapeBuffer = LinkedBuffer.allocate(SerializerBufferSize); // 65MB ;
	Schema<SerializableRecordTape> recordTapeSchema = RuntimeSchema.getSchema(SerializableRecordTape.class);
	
	/**
	 * Serialize {@link SerializableRecordTape} instance.
	 * @param obj
	 * @return byte array if serialization is successful, else empty array.
	 */
	public byte[] serializeObj(SerializableRecordTape obj)
	{
		try
		{
			byte[] data = GraphIOUtil.toByteArray(obj, recordTapeSchema, recordTapeBuffer);
			return data;
		}
		catch(Exception ex)
		{
			sLogger.error("failed to serialize", ex);
		}
		finally
		{
			recordTapeBuffer.clear();
		}
		return null;
	}
	
	/**
	 * Deserializes the byte array data to {@link SerializableRecordTape}
	 * @param data
	 * @return instance of {@link SerializableRecordTape} instance
	 */
	public SerializableRecordTape deSerializeObj(byte[] data)
	{
		SerializableRecordTape deSerObj = recordTapeSchema.newMessage();
		GraphIOUtil.mergeFrom(data, deSerObj, recordTapeSchema);
		return deSerObj;
	}
}
