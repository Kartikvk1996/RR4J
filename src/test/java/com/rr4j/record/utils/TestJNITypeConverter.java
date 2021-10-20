package com.rr4j.record.utils;

import static org.junit.Assert.assertEquals;

import java.util.HashMap;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;

import dnl.utils.text.table.TextTable;

public class TestJNITypeConverter
{
	Map<String, String> jniTypeExpectedValues = new HashMap<String, String>();
	
	String[] title = {"JNI Method", "Expected Java Method", "Converted Java Method"};
	
	@Before
	public void init()
	{
		jniTypeExpectedValues.clear();
		jniTypeExpectedValues.put("methodA()V", "void test.A.methodA()");
		jniTypeExpectedValues.put("methodA()Z", "boolean test.A.methodA()");
		jniTypeExpectedValues.put("methodA()B", "byte test.A.methodA()");
		jniTypeExpectedValues.put("methodA()C", "char test.A.methodA()");
		jniTypeExpectedValues.put("methodA()S", "short test.A.methodA()");
		jniTypeExpectedValues.put("methodA()I", "int test.A.methodA()");
		jniTypeExpectedValues.put("methodA()J", "long test.A.methodA()");
		jniTypeExpectedValues.put("methodA()F", "float test.A.methodA()");
		jniTypeExpectedValues.put("methodA()D", "double test.A.methodA()");
		jniTypeExpectedValues.put("methodA()Ljava/lang/String;", "java.lang.String test.A.methodA()");
		
		jniTypeExpectedValues.put("methodA()V", "void test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[Z", "boolean[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[B", "byte[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[C", "char[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[S", "short[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[I", "int[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[J", "long[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[F", "float[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[D", "double[] test.A.methodA()");
		jniTypeExpectedValues.put("methodA()[Ljava/lang/String;", "java.lang.String[] test.A.methodA()");
		
		jniTypeExpectedValues.put("methodA(I)V", "void test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)Z", "boolean test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)B", "byte test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)C", "char test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)S", "short test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)I", "int test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)J", "long test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)F", "float test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)D", "double test.A.methodA(int)");
		jniTypeExpectedValues.put("methodA(I)Ljava/lang/String;", "java.lang.String test.A.methodA(int)");
		
		jniTypeExpectedValues.put("methodA(IJ)V", "void test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)Z", "boolean test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)B", "byte test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)C", "char test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)S", "short test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)I", "int test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)J", "long test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)F", "float test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)D", "double test.A.methodA(int,long)");
		jniTypeExpectedValues.put("methodA(IJ)Ljava/lang/String;", "java.lang.String test.A.methodA(int,long)");
		
		jniTypeExpectedValues.put("methodA(I[J)V", "void test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)Z", "boolean test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)B", "byte test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)C", "char test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)S", "short test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)I", "int test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)J", "long test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)F", "float test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)D", "double test.A.methodA(int,long[])");
		jniTypeExpectedValues.put("methodA(I[J)Ljava/lang/String;", "java.lang.String test.A.methodA(int,long[])");
		
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)V", "void test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)Z", "boolean test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)B", "byte test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)C", "char test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)S", "short test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)I", "int test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)J", "long test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)F", "float test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)D", "double test.A.methodA(int,java.lang.String[])");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;)Ljava/lang/String;", "java.lang.String test.A.methodA(int,java.lang.String[])");
		
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)V", "void test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)Z", "boolean test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)B", "byte test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)C", "char test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)S", "short test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)I", "int test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)J", "long test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)F", "float test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)D", "double test.A.methodA(int,java.lang.String[],long)");
		jniTypeExpectedValues.put("methodA(I[Ljava/lang/String;J)Ljava/lang/String;", "java.lang.String test.A.methodA(int,java.lang.String[],long)");
	}
	
	@Test
	public void testJniTypeConversion()
	{
		String[][] data = new String[jniTypeExpectedValues.size()][3];
		int ittr = 0;
		
		for(Map.Entry<String,String> entry : jniTypeExpectedValues.entrySet())
		{			
			data[ittr][0] = entry.getKey();
			data[ittr][1] = entry.getValue();
			data[ittr][2] = JNITypeConverter.convert(entry.getKey(), "test.A");
			ittr++;
			assertEquals(entry.getValue(), JNITypeConverter.convert(entry.getKey(), "test.A"));	
		}
		
		TextTable tt = new TextTable(title, data);
		tt.setSort(0);
		tt.setAddRowNumbering(true);
		tt.printTable();
		System.out.println("\n\n");
		return;
	}
	
}
