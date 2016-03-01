TARGET_APP=workMPI

all:
	mpic++ mpi/main.cpp -o ${TARGET_APP}

clean:
	rm -f ${TARGET_APP} *.o *.err *.log
