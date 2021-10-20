package com.rr4j.replay;

import java.util.List;
import java.util.Scanner;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.Options;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.rr4j.record.serialize.SerializableRecordTape;
import com.rr4j.replay.utils.RecordPrinter;

import dnl.utils.text.table.TextTable;

/**
 * <h1>Interactor</h1>
 * 
 * Command line interactor, where user will interact with the tool running in 
 * replay mode.
 * 
 * @author kartik
 *
 */
public class Interactor 
{	
	private static final Logger logger = LogManager.getLogger(Interactor.class);
	private DeSerializer deSerializer;
	private TextTable tt;
	private List<SerializableRecordTape> records;
	private final String dumpFilePath;
	private final String[] columns = {"Command", "Value Required", "Description"};
	private final Object[][] commandInfo = {{"-a","No","dumps all records information."},
											{"-rs","Yes","starts the particular record, ex [-rs 1] will start first record."},
											{"-rsa","No","starts all records one by one."},
											{"-er","No","dumps all records where exception is caught."},
											{"-q","No","to quit."}
											};
	
	private RecordPrinter recordPrinter;
	private RecordExecutorService executorService;
	
	public Interactor(String dumpFilePath) 
	{
		this.dumpFilePath = dumpFilePath;
		tt = new TextTable(columns, commandInfo);
		tt.setSort(0);		
	}
	
	public boolean init()
	{
		logger.info("Initializing interactor");
		deSerializer = new DeSerializer(dumpFilePath);
		if(deSerializer.init())
		{
			deSerializer.deSerialize();
			records = deSerializer.getDeSerializedRecords();
			recordPrinter = new RecordPrinter(records);
			recordPrinter.init();
			executorService = new RecordExecutorService();
			return true;
		}
		return false;
	}
	
	public void start()
	{
		logger.info("Interactor started");
		Options options = new Options();
		options.addOption("a", false, "dumps all records information");
		options.addOption("rs", true, "starts the particular record, ex [-rs 1] will start first record.");
		options.addOption("rsa", false, "starts all records one by one.");
		options.addOption("er", false, "dumps all records where exception is caught");
		options.addOption("q",false,"to quit");
		
		CommandLineParser parser = new DefaultParser();		
		Scanner sc = new Scanner(System.in);
		while(true)
		{
			tt.printTable();
			System.out.println("Provide your input\n");
			String[] inputCommand = sc.nextLine().split(" ");
			try 
			{
				CommandLine cl = parser.parse(options, inputCommand);
				if(cl.hasOption("q"))
				{
					executorService.shutdown();
					return;
				}
				else if(cl.hasOption("a"))
				{
					recordPrinter.printAllRecords();
				}
				else if(cl.hasOption("rs"))
				{
					int index = Integer.valueOf(cl.getOptionValue("rs"));
					
					if(index > records.size() || index <= 0)
						throw new Exception("Provided index value is greater/smaller than available records");
					
					executorService.executeJob(new RecordExecutor(records.get(index-1)));
				}
				else if(cl.hasOption("rsa"))
				{
					for(SerializableRecordTape record : records)
					{
						executorService.executeJob(new RecordExecutor(record));
					}
				}
				else if(cl.hasOption("er"))
				{
					recordPrinter.printExceptionRecords();
				}
				else
				{
					logger.error("Invalid input");
				}
			}
			catch (Exception e) 
			{
				logger.error(e);
			}

		}
	}
}
