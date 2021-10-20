package com.rr4j.record;

import java.util.ArrayDeque;
import java.util.HashSet;
import java.util.Queue;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReentrantLock;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.rr4j.template.MethodCallIn;
import com.rr4j.template.MethodCallOutObject;
import com.rr4j.template.MethodCallOutObjectArr;
import com.rr4j.template.RecordedTape;

public class Record 
{
	private static Logger sLogger = LogManager.getLogger(Record.class);
	private static AtomicInteger currentActiveThreads = new AtomicInteger(0);
	private static Set<Thread> allowedThreads = new HashSet<Thread>();
	private static AtomicInteger totalRecordsRecordedSoFar = new AtomicInteger(0);
	private static ConcurrentHashMap<Thread, RecordedTape> recordInProgress = new ConcurrentHashMap<Thread, RecordedTape>();
	private static Queue<RecordedTape> recordCompleted = new ArrayDeque<RecordedTape>();
	private static ReentrantLock lock = new ReentrantLock();
	
	private static void createMethodInObj(Object thisObject, Object[] inputArgs, int depth, String methodName, String methodClassName)
	{
		Thread t = Thread.currentThread();
		
		RecordedTape template = null;
		if(RecordDirectives.SERIALIZE_THISOBJECT)
		{
			synchronized (thisObject) 
			{
				template = new RecordedTape(t.getName(), thisObject, true);
			}
		}
		else
		{
			template = new RecordedTape(t.getName(), thisObject, false);
		}
		
		MethodCallIn mIn = new MethodCallIn(inputArgs, depth, methodName, methodClassName);
		template.setMethodIn(mIn);
		recordInProgress.put(t, template);
	}
	
	private static void createMethodOutObj(Object rValue, int depth, String methodName, String methodClassName)
	{
		RecordedTape template = recordInProgress.get(Thread.currentThread());
		if(template != null)
		{
			MethodCallOutObject mOutObj = new MethodCallOutObject(rValue, depth, methodName, methodClassName);
			template.setMethodOut(mOutObj);
		}

	}
	
	private static void createMethodOutObj(Object[] rValues, int depth, String methodName, String methodClassName)
	{
		RecordedTape template = recordInProgress.get(Thread.currentThread());
		if(template != null)
		{
			MethodCallOutObjectArr mOutObj = new MethodCallOutObjectArr(rValues, depth, methodName, methodClassName);
			template.setMethodOut(mOutObj);
		}
	}
	
	private static boolean isCurrentThreadAllowed()
	{
		try
		{
			lock.lock();
			if(currentActiveThreads.get() < RecordDirectives.THREADS_TO_RECORD)
			{
				currentActiveThreads.incrementAndGet();
				allowedThreads.add(Thread.currentThread());
				return true;
			}
			else
			{
				if(allowedThreads.contains(Thread.currentThread()) == true)
					return true;
				else
					return false;
			}
		}
		finally
		{
			lock.unlock();
		}
	}
	
	public static void methodIn(Object thisObject, Object[] inputArgs, int depth, String methodClassName, String mName)
	{
		try
		{
			if((RecordDirectives.RECORDING_STATUS == 1) && (RecordDirectives.INITIATE_DUMP == false))
			{
				sLogger.info("Method "+mName+", class "+methodClassName+" isBeingRecorded");
				// Record for future and delete past on overflow.
				if(RecordDirectives.REMEMBER_HISTORY < 0)
				{
					if(totalRecordsRecordedSoFar.get() == Math.abs(RecordDirectives.REMEMBER_HISTORY))
					{
						totalRecordsRecordedSoFar.decrementAndGet();
						// Remove oldest entry
						recordCompleted.poll();
					}
					
					if( isCurrentThreadAllowed())
					{
						totalRecordsRecordedSoFar.incrementAndGet();
						// Add new one
						createMethodInObj(thisObject, inputArgs, depth, mName, methodClassName);
					}
				}
				else if(RecordDirectives.REMEMBER_HISTORY == 0)
				{
					totalRecordsRecordedSoFar.incrementAndGet();
					if(recordInProgress.size() > 0)
					{
						recordInProgress.clear();
					}
					createMethodInObj(thisObject, inputArgs, depth, mName, methodClassName);
					RecordDirectives.RECORDING_STATUS = 2;
				}
				else
				{
					if(totalRecordsRecordedSoFar.get() < RecordDirectives.REMEMBER_HISTORY)
					{
						if(isCurrentThreadAllowed())
						{
							if(totalRecordsRecordedSoFar.incrementAndGet() == RecordDirectives.REMEMBER_HISTORY)
								RecordDirectives.RECORDING_STATUS = 2;
							createMethodInObj(thisObject, inputArgs, depth, mName, methodClassName);
						}
					}
					else
					{
						RecordDirectives.RECORDING_STATUS = 2;
					}
				}	
			}
		}
		catch(Exception ex)
		{
			System.out.println(ex);
		}
		
	}
	
	private static void checkForExit(int depth)
	{
		if(depth == 0)
		{
			Thread t = Thread.currentThread();
			RecordedTape recordTape = recordInProgress.get(t);
			if(recordTape != null)
			{
				recordTape.setThreadName(t.getName());
				recordCompleted.add(recordTape);
				recordInProgress.remove(t);
				recordTape.setRecordCompleted(true);
			}
		}
	}
	
	public static void methodOut(Object thisObject, Object returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, int returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, float returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, double returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, boolean returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, char returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, short returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, long returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, byte returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, Object[] returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, int[] returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, float[] returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, double[] returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, boolean[] returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, char[] returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, short[] returnValue, String methodClassName, int depth, String mName)
	{		
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, long[] returnValue, String methodClassName, int depth, String mName)
	{		
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static void methodOut(Object thisObject, byte[] returnValue, String methodClassName, int depth, String mName)
	{
		if(RecordDirectives.RECORDING_STATUS >= 1)
		{
			checkForExit(depth);
			createMethodOutObj(returnValue, depth, mName, methodClassName);
		}
	}
	
	public static boolean isSafeToSerialize()
	{
		if(recordInProgress.size() > 0)
			return false;
		return true;
	}
	
	public static  Queue<RecordedTape> getRecordInformation()
	{
		if(isSafeToSerialize())
			return recordCompleted;
		return null;
	}
	
	public static void reset()
	{
		currentActiveThreads.set(0);
		allowedThreads.clear();
		totalRecordsRecordedSoFar.set(0);
		recordInProgress.clear();
		currentActiveThreads.set(0);
		recordCompleted.clear();
		allowedThreads.clear();
	}
	
	public static void handleException(Thread t, Exception ex, String className, String methodName)
	{
		RecordedTape tape = recordInProgress.get(t);
		if(tape != null)
		{
			tape.setExceptionCaught(true);
			MethodCallOutObject outObj = new MethodCallOutObject(null, -1, methodName, className);
			outObj.setExceptionClassName(className);
			outObj.setExceptionMethodName(methodName);
			outObj.setException(ex);
			recordInProgress.remove(t);
			recordCompleted.add(tape);
		}
	}
}
