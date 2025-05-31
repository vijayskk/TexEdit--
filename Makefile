CC=g++
CFLAGS = `wx-config --cxxflags --libs`
EXEC=texedit
MAIN=src/main.cpp
ARGS= 

main:src/main.cpp
	$(CC) $? $(CFLAGS) -o build/$(EXEC) 

run:
	unset GTK_PATH
	build/$(EXEC) $(ARGS)

install:
	sudo cp build/$(EXEC) /usr/local/bin

uninstall:
	sudo rm -f /usr/local/bin/$(EXEC)

clear:
	rm build/* 
