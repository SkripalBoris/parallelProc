TARGET_APP=workSerial

all:
	g++ -o ${TARGET_APP} mainSerial.cpp

clean:
	rm -f ${TARGET_APP} *.o *.err *.log
