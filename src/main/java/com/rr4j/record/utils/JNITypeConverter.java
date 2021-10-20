package com.rr4j.record.utils;

/**
 * No longer used.
 * @author kartik
 *
 */
@Deprecated
public class JNITypeConverter
{		
	private static StringBuilder args = new StringBuilder();
	private static StringBuilder rType = new StringBuilder();
	private static StringBuilder mtdName = new StringBuilder();
	
	public static String convert(String method, String klassName)
	{
		args.delete(0, args.length());
		mtdName.delete(0, mtdName.length());
		rType.delete(0, rType.length());
		if(method == null || method.length() == 0)
			return method;
		
		try
		{
			char mtd[] = method.toCharArray();
			int index = 0;
			
			// extract method Name;
			while(mtd[index] != TypeMapper.OPENING_BRACKET)
			{
				index++;
			}
			
			String methodName = method.substring(0, index);
			mtdName.append(methodName);

			args.append(mtd[index++]);
			
			int countArgs = 0;
			
			// Extract all args
			while(mtd[index] != TypeMapper.CLOSING_BRACKET)
			{
				char currentChar = mtd[index];
				String mappedValue = TypeMapper.mapper.get(currentChar);
				if(mappedValue == null) // might be either fully qualified name or array
				{
					if(currentChar == 'L')
					{
						index++;
						// run till we see semicolon to extract arg
						while(mtd[index] != TypeMapper.SEMICOLON)
						{
							if(mtd[index] == '/')
								mtd[index] = '.';
							
							args.append(mtd[index]);
							index++;
						}
					}
					else if(currentChar == '[') // array
					{
						index++;
						if(TypeMapper.mapper.get(mtd[index]) == null) // fully qualified array
						{
							index++;
							while(mtd[index] != TypeMapper.SEMICOLON)
							{
								if(mtd[index] == '/')
									mtd[index] = '.';
								
								args.append(mtd[index]);
								index++;
							}
						}
						else
						{
							args.append(TypeMapper.mapper.get(mtd[index]));
						}
						args.append("[]");
					}
					countArgs++;
				}
				else
				{
					args.append(mappedValue);
					countArgs++;
				}
				if(countArgs>=1)
					args.append(",");
				
				index++;
			}
			
			if(countArgs >= 1)
			{
				args.delete(args.length()-1, args.length());
			}
			
			args.append(')');
			
			// add return type
			index++;
			char currentChar = mtd[index];
			String mappedValue = TypeMapper.mapper.get(currentChar);
			if(mappedValue == null) // might be either fully qualified name or array
			{
				if(currentChar == 'L')
				{
					index++;
					// run till we see semicolon to extract arg
					while(mtd[index] != TypeMapper.SEMICOLON)
					{
						if(mtd[index] == '/')
							mtd[index] = '.';
						
						rType.append(mtd[index]);
						index++;
					}
				}
				else if(currentChar == '[') // array
				{
					index++;
					if(TypeMapper.mapper.get(mtd[index]) == null) // fully qualified array
					{
						index++;
						while(mtd[index] != TypeMapper.SEMICOLON)
						{
							if(mtd[index] == '/')
								mtd[index] = '.';
							
							rType.append(mtd[index]);
							index++;
						}
					}
					else
					{
						rType.append(TypeMapper.mapper.get(mtd[index]));
					}
					rType.append("[]");
				}
			}
			else
			{
				rType.append(mappedValue);
			}
			rType.append(' ');
		}
		catch(Exception ex)
		{
			System.out.println(ex);
			return null;
		}
		
		if(klassName != null)
			rType.append(klassName).append('.').append(mtdName).append(args.toString());
		else
			rType.append(mtdName).append(args.toString());
		
		return rType.toString();
	}
	
}
