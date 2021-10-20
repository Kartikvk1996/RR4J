package com.rr4j.replay;

import java.lang.reflect.Method;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.rr4j.record.serialize.SerializableMethodCallIn;
import com.rr4j.record.serialize.SerializableRecordTape;
import com.rr4j.replay.handles.RecordHandler;
import com.rr4j.replay.utils.MethodParser;

/**
 * <h1>RecordExecutor</h1>
 * 
 * This will replay the recorded tape. From the selected deserialized form of {@link SerializableRecordTape}
 * instance, we first extract the thread name and update the current executing thread name with recorded one,
 * post that a methods executed are registered to replay agent which will mock substitute the actual execution
 * of a particular method with recorded one.
 * 
 * @author kartik
 *
 */
public class RecordExecutor implements Runnable
{
	public static final Logger logger = LogManager.getLogger(RecordExecutor.class);
	private SerializableRecordTape record;
	private MethodParser methodParser;
	
	public RecordExecutor(SerializableRecordTape record) 
	{
		this.record = record;
		methodParser = new MethodParser();
	}
	
	public RecordExecutor() 
	{
	}
	
	public void run()
	{
		logger.info("Starting to execute selected record {}", record);
		Thread executorThread = Thread.currentThread();
		String threadName = executorThread.getName();
		String rTName = record.getThreadName();
		try
		{
			executorThread.setName(rTName);
			Class<?> objClass = record.getThisObject().getClass();
			
			SerializableMethodCallIn sMethodIn = record.getSerializableMethodIn();
			
			// Use method parser to extract arg types and return types.
			methodParser.setMethod(sMethodIn.getMethodName());
			methodParser.parse();

			// Dump some basic information of a record.
			logger.info("Record recorded time : {}", record.getTimeStamp());
			logger.info("Record recorded on thread : {}", record.getThreadName());
			logger.info("Record entry method class name : {}", sMethodIn.getMethodClassName());
			logger.info("Record entry method name : {}", sMethodIn.getMethodName());
			logger.info("Record entry method arg types : {}", sMethodIn.getArgsType());
			logger.info("Record entry method args parsed to proper types : {}", methodParser.getMethodArgsTypesList());
			
			logger.info("Invoking method '{}'", methodParser.getBareMethodName());
			Method method = objClass.getDeclaredMethod(methodParser.getBareMethodName(), methodParser.getMethodArgsTypeArray());
			method.setAccessible(true);
			
			// Don't I repeat Don't change below method position and insert any other method calls, U are doomed
			RecordHandler.registerThread(rTName, executorThread);
			RecordHandler.registerMethods(record.getSerializableMethodOut(), rTName);
			RecordHandler.startMonitoring(rTName, executorThread);
			method.invoke(record.getThisObject(), record.getSerializableMethodIn().getInputArgs());
			
			logger.info("Successfully completed method record execution");
		}
		catch(Exception ex)
		{
			logger.error("Error occurred while executing record", ex);
		}
		finally
		{
			RecordHandler.stopMonitoring(rTName, executorThread);
			RecordHandler.deregisterThread(rTName, executorThread);
			executorThread.setName(threadName);
		}
	}

	public SerializableRecordTape getRecord() 
	{
		return record;
	}

	public void setRecord(SerializableRecordTape record) 
	{
		this.record = record;
	}

	
}
