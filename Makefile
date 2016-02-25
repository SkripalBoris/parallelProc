all:
	make -f makeThreads.mk
	make -f makeSerial.mk

clean:
	make -f makeThreads.mk clean
	make -f makeSerial.mk clean
