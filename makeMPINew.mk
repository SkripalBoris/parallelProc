TARGET_APP=workMPINew

all:
	mpic++ mpiNew/main.cpp -o ${TARGET_APP}

clean:
	rm -f ${TARGET_APP} *.o *.err *.log
