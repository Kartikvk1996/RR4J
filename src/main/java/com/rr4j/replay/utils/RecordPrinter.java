package com.rr4j.replay.utils;

import java.util.Date;
import java.util.List;

import com.rr4j.record.serialize.SerializableRecordTape;

import dnl.utils.text.table.TextTable;

/**
 * <h1>RecordPrinter</h1>
 * 
 * Prints all the record executions in table format.
 * 
 * @author kartik
 *
 */
public class RecordPrinter
{
	private TextTable allRecords;
	private TextTable exceptionRecords;
	private List<SerializableRecordTape> records;
	private String[] columns = {"Index","RecordInstance","ThreadName","Time","ExceptionCaught","Method Entry"};
	private String[][] allRecordsData;
	private String[][] exceptionRecordsData;
	
	public RecordPrinter(List<SerializableRecordTape> records) 
	{
		this.records = records;
		allRecordsData = new String[records.size()][6];
	}
	
	public void init()
	{
		int recordCounter = 0;
		int exceptionCounter = 0;
		for(SerializableRecordTape eachRecord : records)
		{
			allRecordsData[recordCounter][0] = String.valueOf(recordCounter+1);
			allRecordsData[recordCounter][1] = eachRecord.getThisObject().toString();
			allRecordsData[recordCounter][2] = eachRecord.getThreadName();
			allRecordsData[recordCounter][3] = String.valueOf(new Date(eachRecord.getTimeStamp()));
			allRecordsData[recordCounter][4] = String.valueOf(eachRecord.isExceptionCaught());
			allRecordsData[recordCounter][5] = eachRecord.getSerializableMethodIn().getMethodClassName()+"."+eachRecord.getSerializableMethodIn().getMethodName();
			recordCounter++;
			
			if(eachRecord.isExceptionCaught())
			{
				exceptionCounter++;
			}
			
		}
		
		int tempCounter = 0;
		exceptionRecordsData = new String[exceptionCounter][6];
		exceptionCounter = 0;
		for(SerializableRecordTape eachRecord : records)
		{
			if(eachRecord.isExceptionCaught())
			{	
				exceptionRecordsData[tempCounter][0] = String.valueOf(exceptionCounter+1);
				exceptionRecordsData[tempCounter][1] = eachRecord.getThisObject().toString();
				exceptionRecordsData[tempCounter][2] = eachRecord.getThreadName();
				exceptionRecordsData[tempCounter][3] = String.valueOf(new Date(eachRecord.getTimeStamp()));
				exceptionRecordsData[tempCounter][4] = String.valueOf(eachRecord.isExceptionCaught());
				exceptionRecordsData[tempCounter][5] = eachRecord.getSerializableMethodIn().getMethodClassName()+"."+eachRecord.getSerializableMethodIn().getMethodName();

				tempCounter++;
			}
			exceptionCounter++;
		}
		
		allRecords = new TextTable(columns, allRecordsData);
		
		exceptionRecords = new TextTable(columns, exceptionRecordsData);
		
	}
	
	public void printAllRecords()
	{
		allRecords.printTable();
	}
	
	public void printExceptionRecords()
	{
		exceptionRecords.printTable();
	}
}
