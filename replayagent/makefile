# compiler
CC = g++

# all headers files
INCLUDES = /includes

# compiler flags
CFLAGS = -I$(INCLUDES)/ -g -fPIC -fno-omit-frame-pointer -fno-strict-aliasing -std=c++11

# agent name
AGENT = replayagent.cpp

#File compilations
all: cleanObj cleanTgt createTgt objectsGenerate copyToTarget generateAgent createSharedLib

clean: cleanObj cleanTgt

cleanObj:
	rm -f *.o rm -f *.so
	
cleanTgt:
	rm -rf target
	
createTgt:
	mkdir target

objectsGenerate:
	$(CC) $(CFLAGS) -c core/*.cpp

copyToTarget:
	mv *.o ./target/
	
generateAgent:
	$(CC) $(CFLAGS) -c -o replayagent.o replayagent.cpp target/*.o
	
createSharedLib:
	$(CC) $(CFLAGS) -shared -o libreplayagent.so replayagent.o target/*.o
