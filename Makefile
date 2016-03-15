all:
	make -f makeThreads.mk
	make -f makeSerial.mk
	make -f makeMPI.mk

clean:
	make -f makeThreads.mk clean
	make -f makeSerial.mk clean
	make -f makeMPI.mk clean
