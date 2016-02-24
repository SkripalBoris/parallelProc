TARGET_APP=workThreads

all:
	g++ -D_REENTRANT -o ${TARGET_APP} mainThreads.cpp -lpthread

clean:
	rm -f ${TARGET_APP} *.o *.err *.log
