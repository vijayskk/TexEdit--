CC=g++
CFLAGS = `wx-config --cxxflags --libs`
EXEC=run
MAIN=src/main.cpp
ARGS= 

main:src/main.cpp
	$(CC) $? $(CFLAGS) -o build/$(EXEC) 

run:
	unset GTK_PATH
	build/$(EXEC) $(ARGS)

clear:
	rm build/* 
