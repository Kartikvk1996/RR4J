package com.rr4j.replay;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.ArrayList;
import java.util.List;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.rr4j.record.serialize.RecordTapeSerializeDeserialize;
import com.rr4j.record.serialize.SerializableRecordTape;
import com.rr4j.record.serialize.ThisObjectSerializeDeserialize;

/**
 * <h1>DeSerializer</h1>
 * 
 * Deserializes the provided record dump and constructs in-memory representation of 
 * {@link SerializableRecordTape} which is later used by {@link RecordExecutor} to replay the
 * recorded tape.
 * 
 * @author kartik
 *
 */
public class DeSerializer
{
	private static final Logger logger = LogManager.getLogger(DeSerializer.class);
	private RecordTapeSerializeDeserialize objSerDeSer = new RecordTapeSerializeDeserialize();
	private ThisObjectSerializeDeserialize tObjSerDeSer = new ThisObjectSerializeDeserialize();
	private List<byte[]> data;
	private List<SerializableRecordTape> deSerializedRecords;
	private String dumpFilePath;
	
	public DeSerializer(String dumpFilePath)
	{
		logger.info("Initializing DeSerializer");
		this.dumpFilePath = dumpFilePath;
	}
	
	@SuppressWarnings("unchecked")
	public boolean init()
	{
		logger.info("Reading dump file from location {}", dumpFilePath);
		File f = new File(dumpFilePath);
		FileInputStream fin = null;
		ObjectInputStream oIn = null;
		try 
		{
			fin = new FileInputStream(f);
			oIn = new ObjectInputStream(fin);
			data = (List<byte[]>) oIn.readObject();
			logger.info("Successfully read dump file");
			return true;
		}
		catch (FileNotFoundException ex) 
		{
			logger.error("Failed to read the dump file", ex);
		} 
		catch (IOException ex) 
		{
			logger.error("Failed to read the dump file", ex);
		} 
		catch (ClassNotFoundException ex)
		{
			logger.error("Failed to read the dump file", ex);
		}
		finally
		{
			if(fin != null)
			{
				try {fin.close();}
				catch (IOException e) {}
			}
		}
		return false;
	}
	
	public void deSerialize()
	{
		logger.info("Starting to deserialize read data");
		deSerializedRecords = new ArrayList<SerializableRecordTape>();
		for(byte[] recordInBytes : data)
		{
			SerializableRecordTape record = objSerDeSer.deSerializeObj(recordInBytes);
			
			if(record.getThisObject() == null && record.getSerializedThisObject() != null)
			{
				record.setThisObject(tObjSerDeSer.deSerializeObj(record.getSerializedThisObject()).getObject());
			}
			deSerializedRecords.add(record);
		}
		logger.info("Constructed {} in-memory records", deSerializedRecords.size());
	}

	public List<SerializableRecordTape> getDeSerializedRecords() 
	{
		return deSerializedRecords;
	}
	
	
}
