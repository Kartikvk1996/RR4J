package com.rr4j.record.utils;

import java.util.HashMap;

/**
 * <h1>NativeTypeMapper</h1>
 * 
 * Maps type name to type class.
 * 
 * @author kartik
 *
 */
public class NativeTypeMapper
{	
	public static HashMap<String, Class<?>> mapper;

	static
	{
		mapper = new HashMap<String, Class<?>>();
		mapper.put("boolean", boolean.class);
		mapper.put("byte", byte.class);
		mapper.put("char", char.class);
		mapper.put("short", short.class);
		mapper.put("int", int.class);
		mapper.put("long", long.class);
		mapper.put("float", float.class);
		mapper.put("double", double.class);
		mapper.put("void", void.class);
	}
}
