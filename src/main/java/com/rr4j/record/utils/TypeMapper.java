package com.rr4j.record.utils;

import java.util.HashMap;

/**
 * <h1>TypeMapper</h1>
 * 
 * Maps JVM native type to Java type.
 * 
 * More information <a href="https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/types.html">JNI Types</a>
 * 
 * @author kartik
 *
 */
public class TypeMapper 
{
	public static final char OPENING_BRACKET = '(';
	public static final char OPENING_SQUARE_BRACKET = '[';
	public static final char CLOSING_BRACKET = ')';
	public static final char SEMICOLON = ';';
	
	public static HashMap<Character, String> mapper;

	static
	{
		mapper = new HashMap<Character, String>();
		mapper.put('Z', "boolean");
		mapper.put('B', "byte");
		mapper.put('C', "char");
		mapper.put('S', "short");
		mapper.put('I', "int");
		mapper.put('J', "long");
		mapper.put('F', "float");
		mapper.put('D', "double");
		mapper.put('V', "void");
	}
}
