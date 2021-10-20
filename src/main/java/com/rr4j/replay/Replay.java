package com.rr4j.replay;

import java.io.PrintWriter;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * <h1>Replay</h1>
 * 
 * Main Entry point for a tool running in replay mode.
 * 
 * @author kartik
 *
 */
public class Replay 
{
	private static final Logger logger = LogManager.getLogger(Replay.class);
	
	public static void main(String[] args) throws ParseException 
	{
		logger.info("Starting RecordReplay tool in 'Replay' mode");
		Options options = new Options();
		
		// Use apache cli to configure and later extract dumpFile path
		Option dumpFileOption = Option.builder()
				.longOpt("dumpFile")
				.argName("file path")
				.hasArg()
				.desc("Dump file path")
				.build();
		
		options.addOption(dumpFileOption);
		CommandLineParser parser = new DefaultParser();
		CommandLine cl = parser.parse(options, args);
		
		if(cl.hasOption("dumpFile"))
		{
			Interactor interactor = new Interactor(cl.getOptionValue("dumpFile"));
			if(interactor.init())
				interactor.start();
			else
			{
				logger.error("Failed to start the interactor");
			}
		}
		else
		{
			HelpFormatter formatter = new HelpFormatter();
			final PrintWriter writer = new PrintWriter(System.out);
			formatter.printUsage(writer,80,"RecordReplay", options);
			writer.flush();
		}
	}

}
