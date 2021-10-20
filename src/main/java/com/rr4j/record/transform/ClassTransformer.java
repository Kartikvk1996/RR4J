package com.rr4j.record.transform;

import java.util.Map.Entry;
import java.util.TreeMap;

import com.rr4j.record.RecordDirectives;

import javassist.ClassClassPath;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.CtMethod;
import javassist.Modifier;

/**
 * <h1>ClassTransformer<h1>
 * 
 * Transforms the class and injects the custom method to record.
 * 
 * @author kartik
 *
 */
public class ClassTransformer 
{	
	private  static ClassPool pool = ClassPool.getDefault();
	private static TreeMap<String, byte[]> oldByteCodeBackup = new TreeMap<String, byte[]>();
	private static Entry<String, byte[]> tempBuffer;
	
	/**
	 * Trasnsforms the class at runtime.
	 * @param klass class 
	 * @param method method name
	 * @param depth depth
	 * @return transformed class bytecode.
	 */
	public static byte[] transformClass(Class<?> klass, String method, int depth)
	{
		// skip constructors
		if(method.contains("<init>"))
		{
			return null;
		}
		
		if(RecordDirectives.IS_ACTIVE_RULE_SET)
		{
			try
			{
				CtClass kls = pool.get(klass.getName());
				kls.defrost();
				CtMethod[] classMethods = kls.getDeclaredMethods();
				for(CtMethod mtd : classMethods)
				{
					byte[] newByteCode = null;
					if(mtd.toString().contains(method))
					{
						String klassName = klass.getName();
						System.out.println(method +"  "+depth);
						
						byte[] oldCode = kls.toBytecode();
						kls.defrost();
						
						// If method is static then 'this' object is absent.
						if(Modifier.isStatic(mtd.getModifiers()))
						{
							if(depth == 0)
							{
								mtd.insertBefore("{com.recordreplay.record.Record.methodIn(null, $args, 0, \""+klassName+"\", \""+method+"\");}");
								mtd.insertAfter("{com.recordreplay.record.Record.methodOut(null, $_, \""+klassName+"\", "+depth+", \""+method+"\");}");
								newByteCode = kls.toBytecode();
							}
							else
							{
								mtd.insertAfter("{com.recordreplay.record.Record.methodOut(null, $_, \""+klassName+"\", "+depth+", \""+method+"\");}");
								newByteCode = kls.toBytecode();
							}
						}
						else
						{
							if(depth == 0)
							{
								mtd.insertBefore("{com.recordreplay.record.Record.methodIn($0, $args, 0, \""+klassName+"\", \""+method+"\");}");
								mtd.insertAfter("{com.recordreplay.record.Record.methodOut($0, $_, \""+klassName+"\", "+depth+", \""+method+"\");}");
								newByteCode = kls.toBytecode();
							}
							else
							{
								mtd.insertAfter("{com.recordreplay.record.Record.methodOut($0, $_, \""+klassName+"\", "+depth+", \""+method+"\");}");
								newByteCode = kls.toBytecode();
							}
						}
		
						if(oldByteCodeBackup.containsKey(method) == false)
						{
							oldByteCodeBackup.put(method, oldCode);
						}
						return newByteCode;
					}
				}
			}
			catch(Exception ex)
			{
				// stop further transformation on a rule.
				//RecordDirectives.resetDirectives();
			}

		}
		return null;
	}
	
	/**
	 * @return Method name to restore.
	 */
	public static String getMtdNameToRestore()
	{
		if(tempBuffer != null)
			return tempBuffer.getKey();
		else
			return null;
	}
	
	/**
	 * @return Old byte code to restore old behavior.
	 */
	public static byte[] getByteCodeToRestore()
	{
		if(oldByteCodeBackup.size() > 0)
		{
			tempBuffer = oldByteCodeBackup.pollFirstEntry();
			return tempBuffer.getValue();
		}
		{
			tempBuffer = null;
			return null;
		}
	}
	
	/**
	 * Adds classpath, used in tomcat server since there are different 
	 * classloaders.
	 * @param o
	 */
	public static void addClassPath(Object o)
	{
		try
		{
			pool.insertClassPath(new ClassClassPath(o.getClass()));
		}
		catch(Exception ex)
		{
		}
	}
}
