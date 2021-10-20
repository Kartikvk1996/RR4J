package com.rr4j.replay.handles;

import java.util.List;

import com.rr4j.record.serialize.SerializableMethodCallOutObject;
import com.rr4j.record.serialize.SerializableMethodCallOutObjectArr;
import com.rr4j.replay.utils.MethodParser;

public class RecordHandler 
{
	private static native void NativeCall_registerThread(String threadName, Thread thread);
	private static native void NativeCall_registerMethodRBoolean(String threadName, String metodName, String className, boolean returnType, int depth);
	private static native void NativeCall_registerMethodRByte(String threadName, String metodName, String className, byte returnType, int depth);
	private static native void NativeCall_registerMethodRChar(String threadName, String metodName, String className, char returnType, int depth);
	private static native void NativeCall_registerMethodRShort(String threadName, String metodName, String className, short returnType, int depth);
	private static native void NativeCall_registerMethodRInt(String threadName, String metodName, String className, int returnType, int depth);
	private static native void NativeCall_registerMethodRLong(String threadName, String metodName, String className, long returnType, int depth);
	private static native void NativeCall_registerMethodRFloat(String threadName, String metodName, String className, float returnType, int depth);
	private static native void NativeCall_registerMethodRDouble(String threadName, String metodName, String className, double returnType, int depth);
	private static native void NativeCall_registerMethodRVoid(String threadName, String metodName, String className,  int depth);
	private static native void NativeCall_registerMethodRObject(String threadName, String metodName, String className, Object returnType, int depth);
	private static native void NativeCall_deregisterThread(String threadName, Thread thread);
	private static native void NativeCall_startMonitoring(String threadName, Thread thread);
	private static native void NativeCall_stopMonitoring(String threadName, Thread thread);
	
	public static void registerMethods(List<?> methodOutObjects, String threadName)
	{
		MethodParser parser = new MethodParser();
		for(Object mtdOut : methodOutObjects)
		{
			if(mtdOut instanceof SerializableMethodCallOutObject)
			{
				SerializableMethodCallOutObject mOutObj = (SerializableMethodCallOutObject) mtdOut;
				parser.setMethod(mOutObj.getMethodName());
				parser.parse();
				Class<?> rType = parser.getMethodReturnType();
				if(mOutObj.isReturnTypeVoid())
				{
					NativeCall_registerMethodRVoid(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), mOutObj.getMethodDepth());
				}
				else
				{
					Object rValue = mOutObj.getMethodReturnValue();
					
					if(rType == boolean.class)
						NativeCall_registerMethodRBoolean(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (boolean) rValue, mOutObj.getMethodDepth());
					else if(rType == byte.class)
						NativeCall_registerMethodRByte(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (byte) rValue, mOutObj.getMethodDepth());
					else if(rType == char.class)
						NativeCall_registerMethodRChar(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (char) rValue, mOutObj.getMethodDepth());
					else if(rType == short.class)
						NativeCall_registerMethodRShort(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (short) rValue, mOutObj.getMethodDepth());
					else if(rType == int.class)
						NativeCall_registerMethodRInt(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (int) rValue, mOutObj.getMethodDepth());
					else if(rType == long.class)
						NativeCall_registerMethodRLong(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (long) rValue, mOutObj.getMethodDepth());
					else if(rType == float.class)
						NativeCall_registerMethodRFloat(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (float) rValue, mOutObj.getMethodDepth());
					else if(rType == double.class)
						NativeCall_registerMethodRDouble(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), (double) rValue, mOutObj.getMethodDepth());
					else
						NativeCall_registerMethodRObject(threadName, mOutObj.getMethodName(), mOutObj.getMethodClassName(), rValue, mOutObj.getMethodDepth());
						
				}
			}
			else
			{
				SerializableMethodCallOutObjectArr mOutObjArr = (SerializableMethodCallOutObjectArr) mtdOut;
				parser.setMethod(mOutObjArr.getMethodName());
				parser.parse();
				Class<?> rType = parser.getMethodReturnType();
				if(mOutObjArr.isReturnTypeVoid())
				{
					NativeCall_registerMethodRVoid(threadName, mOutObjArr.getMethodName(), mOutObjArr.getMethodClassName(), mOutObjArr.getMethodDepth());
				}
				else
				{
					Object rValue = mOutObjArr.getMethodReturnValue();
					NativeCall_registerMethodRObject(threadName, mOutObjArr.getMethodName(), mOutObjArr.getMethodClassName(), rValue, mOutObjArr.getMethodDepth());
				}
			}
		}
	}
	
	public static void registerThread(String threadName, Thread thread) 
	{
		NativeCall_registerThread(threadName, thread);
	}
	
	public static void deregisterThread(String threadName, Thread thread) 
	{
		NativeCall_deregisterThread(threadName, thread);
	}
	
	public static void startMonitoring(String threadName, Thread thread)
	{
		NativeCall_startMonitoring(threadName, thread);
	}
	
	public static void stopMonitoring(String threadName, Thread thread)
	{
		NativeCall_stopMonitoring(threadName, thread);
	}
}
