package com.rr4j.record.serialize;

import java.io.File;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Queue;

import com.rr4j.record.RecordDirectives;
import com.rr4j.template.RecordedTape;
import com.rr4j.record.Record;

/**
 * <h1>Serializer</h1>
 * 
 * Serializes the recorded object, first it waits till the recording 
 * process is completed, post that initiates recording dumping process.
 * 
 * @author kartik
 *
 */
public class Serializer 
{
	/**
	 * Serializes the execution recorded.
	 * @param recordDumpPath Recorded dump location.
	 * @return true if serialization is completed, else false on failure.
	 */
	public static boolean serializeData(String recordDumpPath)
	{
		// Wait till the recording is completed.
		while(Record.isSafeToSerialize() == false)
		{
			try 
			{
				Thread.sleep(1000);
			}
			catch (InterruptedException e) 
			{}
		}
		
		try
		{
			/* Start serializing once the recording is completed and initiate dump command 
			 * is set to true.
			 */
			if(RecordDirectives.RECORDING_STATUS == 2 && RecordDirectives.INITIATE_DUMP == true)
			{
				RecordTapeSerializeDeserialize objSer = new RecordTapeSerializeDeserialize();
				List<byte[]> serializedRecords = new ArrayList<byte[]>();
				Queue<RecordedTape> records = Record.getRecordInformation();
				
				// Iterate over all RecordedTape and serialize each executions.
				for(RecordedTape r : records)
				{
					SerializableRecordTape serRecordTape = new SerializableRecordTape();
					serRecordTape.construct(r);
					serializedRecords.add(objSer.serializeObj(serRecordTape));
				}
				
				// compiler will take care of converting to StringBuffer append
				String dumpName = recordDumpPath+"/recordDump-"+System.currentTimeMillis()+".dmp";
				File f = new File(dumpName);
				f.createNewFile();
				FileOutputStream fout = null;
				ObjectOutputStream objOutStream = null;
				try
				{
					fout = new FileOutputStream(f);
					objOutStream = new ObjectOutputStream(fout);
					objOutStream.writeObject(serializedRecords);
					objOutStream.close();
				}
				finally
				{
					if(fout != null)
						fout.close();
				}
				return true;
			}
		}
		catch(Exception ex)
		{
			return false;
		}
		finally
		{
			RecordDirectives.IS_DUMP_COMPLETED =  true;
		}
		return false;
	}
}
