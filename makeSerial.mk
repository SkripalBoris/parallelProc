TARGET_APP=workSerial

all:
	g++ -o ${TARGET_APP} serial/main.cpp

clean:
	rm -f ${TARGET_APP} *.o *.err *.log
