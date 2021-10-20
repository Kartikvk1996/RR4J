package com.rr4j.replay;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Executor service which will execute each selected record on a thread.
 * 
 * @author kartik
 *
 */
public class RecordExecutorService 
{
	private static final Logger logger = LogManager.getLogger(RecordExecutorService.class);
	private static final int THREADS_COUNT;
	private ExecutorService service;
	
	static
	{
		THREADS_COUNT = Integer.valueOf(System.getProperty("replay.executor.threads", "5"));
	}
	
	public RecordExecutorService() 
	{
		service = new ThreadPoolExecutor(THREADS_COUNT, THREADS_COUNT, 5, TimeUnit.MINUTES, 
				new LinkedBlockingQueue<Runnable>());
	}
	
	/**
	 * Execute a job of type {@link RecordExecutor}
	 * @param record
	 */
	public void executeJob(RecordExecutor record)
	{
		logger.info("Submitting job to service");
		Future<?> result = service.submit(record);
		try 
		{
			logger.info("Waiting for record execution to complete");
			result.get();
		}
		catch (InterruptedException e) 
		{
			logger.error("Failed waiting for record execution completion", e);
		}
		catch (ExecutionException e) 
		{
			logger.error("Failed waiting for record execution completion", e);
		}
	}
	
	public void shutdown()
	{
		service.shutdown();
	}
}
