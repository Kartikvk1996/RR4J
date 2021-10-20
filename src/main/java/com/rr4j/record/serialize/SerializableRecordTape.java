package com.rr4j.record.serialize;

import java.util.ArrayList;
import java.util.List;

import com.rr4j.template.MethodCallOutObject;
import com.rr4j.template.MethodCallOutObjectArr;
import com.rr4j.template.RecordedTape;

/**
 * <h1>SerializableRecordTape</h1>
 * 
 * Serializable form of {@link RecordedTape}
 *  
 * @author kartik
 *
 */
public class SerializableRecordTape
{
	// Thread name which executed.
	private String threadName;
	
	// Timestamp the Record tape is created
	private long timeStamp;
	
	// Method that is first called by thread.
	private SerializableMethodCallIn serializableMethodIn;
	
	// List of method whose return value are stored.
	private List serializableMethodOut = new ArrayList();
	
	private boolean isRecordCompleted = false;
	private boolean isExceptionCaught = false;
	
	// instance that stores the this object.
	private Object thisObject;
	
	// serialzed vertion of thisObject
	private byte[] serializedThisObject;
	
	private int recordHistory;
	private boolean isExitOnException;
	private int maxDepth;
	
	/**
	 * Constructs the serializable form of recorded tape from {@link RecordedTape}
	 * @param recordTape
	 */
	public void construct(RecordedTape recordTape)
	{
		threadName = recordTape.getThreadName();
		timeStamp = recordTape.getTimeStamp();
		serializableMethodIn = new SerializableMethodCallIn();
		serializableMethodIn.construct(recordTape.getMethodIn());
		
		isRecordCompleted = recordTape.isRecordCompleted();
		isExceptionCaught = recordTape.isExceptionCaught();
		thisObject = recordTape.getThisObject();
		serializedThisObject = recordTape.getSerializedThisObject();
		recordHistory = recordTape.getRecordHistory();
		isExitOnException = recordTape.isExitOnException();
		maxDepth = recordTape.getMaxDepth();
		
		List mOut = recordTape.getMethodOutList();
		for(Object o : mOut)
		{
			if(o instanceof MethodCallOutObject)
			{
				SerializableMethodCallOutObject sOut = new  SerializableMethodCallOutObject();
				sOut.construct((MethodCallOutObject) o);
				serializableMethodOut.add(sOut);
			}
			else
			{
				SerializableMethodCallOutObjectArr sOutArr = new  SerializableMethodCallOutObjectArr();
				sOutArr.construct((MethodCallOutObjectArr) o);
				serializableMethodOut.add(sOutArr);
			}
		}		
	}

	/**
	 * @return thread name
	 */
	public String getThreadName() 
	{
		return threadName;
	}

	/**
	 * @return timestamp
	 */
	public long getTimeStamp() 
	{
		return timeStamp;
	}

	/**
	 * @return method in, which is first called by thread.
	 */
	public SerializableMethodCallIn getSerializableMethodIn() 
	{
		return serializableMethodIn;
	}

	/**
	 * @return List of method called.
	 */
	public List getSerializableMethodOut() 
	{
		return serializableMethodOut;
	}

	/**
	 * @return true, if record is completed, else false.
	 */
	public boolean isRecordCompleted() 
	{
		return isRecordCompleted;
	}

	/**
	 * @return true, if exception is caught, else false.
	 */
	public boolean isExceptionCaught() 
	{
		return isExceptionCaught;
	}

	/**
	 * @return caller object instance.
	 */
	public Object getThisObject() 
	{
		return thisObject;
	}

	/**
	 * @return max recorded instances.
	 */
	public int getRecordHistory() 
	{
		return recordHistory;
	}

	/**
	 * @return true, if exit on exception is set, else false.
	 */
	public boolean isExitOnException() 
	{
		return isExitOnException;
	}

	/**
	 * @return max execution depth set.
	 */
	public int getMaxDepth() 
	{
		return maxDepth;
	}

	/**
	 * @return serialized form of this object instance.
	 */
	public byte[] getSerializedThisObject()
	{
		return serializedThisObject;
	}

	/**
	 * Sets this object instance.
	 * @param thisObject
	 */
	public void setThisObject(Object thisObject)
	{
		this.thisObject = thisObject;
	}

	/**
	 * Sets serialized form of this object instance
	 * @param serializedThisObject
	 */
	public void setSerializedThisObject(byte[] serializedThisObject)
	{
		this.serializedThisObject = serializedThisObject;
	}
}
