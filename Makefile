WINLIB = 

ifeq ($(OS), Windows_NT)
	
	WINLIB = -lws2_32	

endif



UNIXLIB = -pthread 

ifeq ($(OS), Windows_NT)
	
	UNIXLIB =	

endif



SOURCES = main.cpp async.cpp fs.cpp timer.cpp





all: 
	g++ -o main  $(SOURCES) $(UNIXLIB) $(WINLIB)
