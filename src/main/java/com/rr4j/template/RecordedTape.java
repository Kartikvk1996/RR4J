package com.rr4j.template;

import java.util.ArrayList;
import java.util.List;

import com.rr4j.record.serialize.ThisObject;
import com.rr4j.record.serialize.ThisObjectSerializeDeserialize;

/**
 * <h1>RecordedTape</h1>
 * 
 * RecordedTape represents the same info we had in 90's tape recorder. where the 
 * songs were recorded and a player used to play it. Same happens in this case.
 * For each execution a RecordedTape is created and later replayed by replay player.a
 * 
 * @author kartik
 *
 */
public class RecordedTape 
{
	private static final ThisObject obj = new ThisObject();
	private static final ThisObjectSerializeDeserialize tObjSDeSer = new ThisObjectSerializeDeserialize();
	
	// Thread name
	private String threadName;
	
	// Time the tape was created.
	private long timeStamp;
	
	// First method called by thread.
	private MethodCallIn methodIn;
	
	// List of methods falls within the depth
	private List methodOut = new ArrayList();
	
	private boolean isRecordCompleted = false;
	
	private boolean isExceptionCaught = false;
	private Object thisObject = null;
	private byte[] serializedThisObject = null;
	private int recordHistory;
	private boolean isExitOnException;
	private int maxDepth;
	
	/**
	 * Constructor.
	 * @param threadName
	 * @param thisObject
	 * @param serializeThis
	 */
	public RecordedTape(String threadName, Object thisObject, boolean serializeThis)
	{
		timeStamp = System.currentTimeMillis();
		this.threadName = threadName;
		if(serializeThis == false)
		{
			this.thisObject = thisObject;
		}
		else
		{
			obj.setObject(thisObject);
			serializedThisObject = tObjSDeSer.serializeObj(obj);
		}
	}

	/**
	 * @return Thread name
	 */
	public String getThreadName() 
	{
		return threadName;
	}
	
	/**
	 * Sets thread name
	 * @param threadName
	 */
	public void setThreadName(String threadName) 
	{
		this.threadName = threadName;
	}
	
	/**
	 * @return record timestamp
	 */
	public long getTimeStamp()
	{
		return timeStamp;
	}
	
	/**
	 * Set record timestamp.
	 * @param timeStamp
	 */
	public void setTimeStamp(long timeStamp) 
	{
		this.timeStamp = timeStamp;
	}
	
	/**
	 * @return initial entry method.
	 */
	public MethodCallIn getMethodIn() 
	{
		return methodIn;
	}
	
	/**
	 * Sets entry method
	 * @param methodIn
	 */
	public void setMethodIn(MethodCallIn methodIn)
	{
		this.methodIn = methodIn;
	}

	/**
	 * @return true if any matching exception caught during execution.
	 */
	public boolean isExceptionCaught() 
	{
		return isExceptionCaught;
	}

	/**
	 * @param isExceptionCaught
	 */
	public void setExceptionCaught(boolean isExceptionCaught) 
	{
		this.isExceptionCaught = isExceptionCaught;
	}

	/**
	 * @return List of all methods called till max depth.
	 */
	public List getMethodOutList() 
	{
		return methodOut;
	}

	/**
	 * Add all the methods called during the record till max depth configured.
	 * @param mOut
	 */
	public void setMethodOut(Object mOut) 
	{
		methodOut.add(mOut);
	}

	/**
	 * @return true, when record is completed. else false is returned.
	 */
	public boolean isRecordCompleted() 
	{
		return isRecordCompleted;
	}

	/**
	 * Sets whether record is completed or not.
	 * @param isRecordCompleted
	 */
	public void setRecordCompleted(boolean isRecordCompleted) 
	{
		this.isRecordCompleted = isRecordCompleted;
	}

	/**
	 * @return 'this' object
	 */
	public Object getThisObject()
	{
		return thisObject;
	}

	/**
	 * Sets 'this' object
	 * @param thisObject
	 */
	public void setThisObject(Object thisObject)
	{
		this.thisObject = thisObject;
	}

	/**
	 * @return record history
	 */
	public int getRecordHistory()
	{
		return recordHistory;
	}

	/**
	 * Sets record history.
	 * @param recordHistory
	 */
	public void setRecordHistory(int recordHistory) 
	{
		this.recordHistory = recordHistory;
	}

	/**
	 * @return true if particular execution generated any exception, else false.
	 */
	public boolean isExitOnException() 
	{
		return isExitOnException;
	}

	/**
	 * Sets exit on exception.
	 * @param isExitOnException
	 */
	public void setExitOnException(boolean isExitOnException) 
	{
		this.isExitOnException = isExitOnException;
	}

	/**
	 * @return maximum depth
	 */
	public int getMaxDepth() 
	{
		return maxDepth;
	}

	/**
	 * Sets maximum record depth
	 * @param maxDepth
	 */
	public void setMaxDepth(int maxDepth) 
	{
		this.maxDepth = maxDepth;
	}

	public byte[] getSerializedThisObject() 
	{
		return serializedThisObject;
	}

	public void setSerializedThisObject(byte[] serializedThisObject) 
	{
		this.serializedThisObject = serializedThisObject;
	}
}
