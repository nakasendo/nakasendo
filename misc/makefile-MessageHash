CC=g++
PATH_TO_SSL = /home/oboxes/secLibFroWasm
CFLAGS =-I. -I. -l:libssl.a -l:libcrypto.a -ldl -fpic -std=c++17 -g -pthread
OBJHASH = MessageHash.o MessageHashImpl.o MessageHashAPI.o
OBJHASHCLIENT = MessageHashClient.o

%.o : %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)


HashTest:$(OBJHASH) $(OBJHASHCLIENT)
	$(CC) -o $@ $^ $(CFLAGS)

HashShared:$(OBJHASH)
	$(CC) -shared -o  $@.so $^ $(CFLAGS)

HashPyModule: MessageHashPythonAPI.cpp
	python MessageHashAPISetup.py build

HashPyModuleInstall:
	python MessageHashAPISetup.py install

clean:
	rm -rf MessageHash.o MessageHashImpl.o MessageHashAPI.o MessageHashClient.o HashTest HashShared.so MessageHash.wast MessageHash.wasm.map MessageHash.wasm MessageHash.js
	rm -rf build

all: HashTest HashShared HashPyModule
install: HashPyModuleInstall
