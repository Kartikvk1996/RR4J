# RR4J [Record Replay 4 Java]
<img width="200" height="200" src="https://github.com/Kartikvk1996/RR4J/blob/main/img/RR4J-logo.jpeg">
RR4J is a tool that records java execution and later allows developers to replay locally. The tool solves one of the challenges
developers faces when developing an application i.e sporadic bugs which are not replicable locally or system-dependent bugs which are hard to reproduce and given a micro-service architecture being incorporated everywhere, It adds an additional overhead of setting up all the services to replicate the issue and then fix it.

## Build Requirements
1. C++ 11
2. Java 8
3. make
4. maven

## Building Record Agent
1. Navigate to recordagent folder
2. Run `make all` command

## Building Replay Agent
1. Navigate to replayagent folder
2. Run `make all` command

## Building Java code
1. Run `mvn clean install`

## Record Agent Configuration
Once the build is completed you need to configure the record agent, this can be done through **config.json** file and the file should be in the same place the agent is. Below is a sample configuration.

```json
{
	"logFileName": "record.log",
	"logFileMaxSize": 100000,
	"maxLogFiles": 2,
	"recordDumpLocation":"",
	"delayedStart":5000,
	"recordCallerSnapshot": true,
	"blacklistPackages":["java/lang","sun/reflect","java/util", "java/io"],
	"rules": [
		{
			"method": "com/example/Test::MethodHasBug(IILjava/lang/String;)Ljava/lang/String;",
			"exception": "java.lang.NullPointerException",
			"additivity": false,
			"exitOnException": true,
			"threadsToMonitor" :1,
			"depth":2,
			"history":10,
			"threadsToRecord":10
		}
	]
}

```

| Configurations       | Usage                                                                                                                                                                                                                                                                                                                                                                                                         |
|----------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| logFileName          | Name of the log file which will be generated<br>by record agent.                                                                                                                                                                                                                                                                                                                                              |
| logFileMaxSize       | Maximum log file size in bytes                                                                                                                                                                                                                                                                                                                                                                                |
| maxLogFiles          | Maximum log files                                                                                                                                                                                                                                                                                                                                                                                             |
| recordDumpLocation   | Record dump location, if empty dump will be<br>generated in the same location agent is present.                                                                                                                                                                                                                                                                                                               |
| delayedStart         | Delay start the record value in milliseconds                                                                                                                                                                                                                                                                                                                                                                  |
| recordCallerSnapshot | True if you want to record 'this' object also, This<br>is useful when method execution updating instance variables.                                                                                                                                                                                                                                                                                           |
| blacklistPackages    | List of packages you want to blacklist                                                                                                                                                                                                                                                                                                                                                                        |
| rules                | Limited feature, currently only one rule is executed.                                                                                                                                                                                                                                                                                                                                                         |
| method               | Method name you want to record. Type : **className::methodName(args)returnType**<br>**Note** '.' (dot) should be replaced with '/' (slash). More info refer : https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/types.html                                                                                                                                                                      |
| exception            | Exception if you are looking for, useful when the issue is intermittent.<br>Exception should be given in fully qualified java name, ex: java.lang.NullPointerException                                                                                                                                                                                                                                        |
| additivity           | **Not Implemented**                                                                                                                                                                                                                                                                                                                                                                                           |
| exitOnException      | True if you want to exit immediately after the exception is caught. Else false.                                                                                                                                                                                                                                                                                                                               |
| threadsToMonitor     | **Default is 1** Don't change the value                                                                                                                                                                                                                                                                                                                                                                       |
| depth                | Maximum depth you want to record.<br>```<br>methodA()<br><br>methodB()<br><br>methodC()<br><br>```<br><br>methodA() is at depth 1 and methodB() is at depth 2, methodC() is not recorded its return<br>value will be impersonated. This is useful if you don't bother about a particular function<br>at certain depth. Ex: DB call, Network Call. You are only interested in the return value.                |
| history              | 1. '0' only records one iteration which satisfies the rule<br>2. positive value, ex 10, 20, etc. Records the number of iteration provided.<br>3. A negative value, ex -10. This is like a sliding window where new records are added and previous records are discarded. This is useful<br>when you are looking for an exception. **exitOnException must be set to true** else the recording process will never be completed.  |
| threadsToRecord      | A Total number of threads to record in a multi-threaded environment. Suppose if the system has 10 threads and the value provided is 3, Only <br>first 3 threads that execute the method will be allowed now and also in the future.                                                                                                                                                                                         |

## Example
### Recording the execution
Consider a simple java code, where I want to record a **methodA(long time)** execution. I will configure my record agent to record 3 iterations with below config values.
```
"method": "Dummy::methodA(J)I",
"exception": "",
"additivity": false,
"exitOnException": false,
"threadsToMonitor" :1,
"depth":2,
"history":3,
"threadsToRecord":1
```

```
class Dummy
{
	int methodA(long time)
	{
		String sTime = String.valueOf(time);
		int val = methodB(sTime);
		return val-1;
		
	}
	
	int methodB(String s)
	{
		int x = methodC().length();
		return s.length()-x;
	}
	
	String methodC()
	{
		String x = "hello world";
		return x;
	}
	
}

public class TestClass
{

	public static void main(String[] args) 
	{
		Dummy d = new Dummy();
		for(int i=0; i<30; i++)
		{
			try {Thread.sleep(1000);} catch(Exception ex) {}
			d.methodA(System.currentTimeMillis());
		}
	}

}
```

Once done, Start a program with the below command.

```
export LD_LIBRARY_PATH=<path/to/record/agent/so/file>
java -cp recordreplay-0.0.1-SNAPSHOT-jar-with-dependencies.jar:**TestClass class file path** -agentlib:recordagent TestClass
```

Post this you will see a dump file generated with the name **recordDump-<timestamp>.dmp** which can be passed as input during replay mode.

### Replaying the execution
Run the following command to start the tool in replay mode.

```
export LD_LIBRARY_PATH=<path/to/replay/agent/so/file>
java -agentlib:replayagent -cp recordreplay-0.0.1-SNAPSHOT-jar-with-dependencies.jar:**TestClass class file path** com.rr4j.replay.Replay --dumpFile recordDump.dmp
```
You will see an interactor, you can also add `-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=8000` to start the debugger and attach an IDE.
  
To list all records use '**-a**' command
  
![image](https://user-images.githubusercontent.com/21128320/138150183-6d135563-1ed1-4f8e-a4e8-86066fbf5d79.png)

You can add a breakpoint in IDE and use '**-rs recordIndex**' to replay the particular record.
  
![image](https://user-images.githubusercontent.com/21128320/138150527-2533bbec-5c8f-4f22-ae77-47cf10e35643.png)

  
## Future scope
1. Record thread state to provide a detailed breakdown, mimic race conditions.
2. Lamda function support.
3. Dynamic class support generated on the fly by ASM and other libraries.
4. Support controlling execution flow, recording only the particular path through line numbers.
