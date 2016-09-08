CV = -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv`
CORE = -lopencv_core
GUI = -lopencv_highgui
PROC = -lopencv_imgproc
CC = g++
C++ = -std=c++11

challenge3: challenge3.o brightfilter.o  find4corner.o transform.o cut.o 
	$(CC) -o challenge3 challenge3.o brightfilter.o find4corner.o transform.o cut.o $(CV) $(CORE) $(GUI) $(C++)
challenge3.o: challenge3.cpp
	$(CC) -c challenge3.cpp $(CV) $(CORE) $(GUI) $(C++)
brightfilter.o: brightfilter.cpp
	$(CC) -c brightfilter.cpp $(CV) $(CORE) $(GUI) $(PROC) $(C++)
find4corner.o: find4corner.cpp 
	$(CC) -c find4corner.cpp $(CV) $(CORE) $(GUI) $(PROC) $(C++)
transform.o: transform.cpp
	$(CC) -c transform.cpp $(CV) $(CORE) $(GUI) $(PROC) $(C++)
cut.o: cut.cpp
	$(CC) -c cut.cpp $(CV) $(CORE) $(GUI) $(C++)

revise: revise.cpp
	$(CC) revise.cpp $(CV) $(CORE) $(GUI) -o revise $(C++)
opencamera: opencamera.cpp
	$(CC) opencamera.cpp $(CV) $(CORE) $(GUI) -o opencamera $(C++)
find4corner: find4corner.cpp
	$(CC) find4corner.cpp $(CV) $(CORE) $(GUI) $(PROC) -o find4corner $(C++)
transform: transform.cpp
	$(CC) transform.cpp $(CV) $(CORE) $(GUI) $(PROC) -o transform $(C++)
cut: cut.cpp
	$(CC) cut.cpp $(CV) $(CORE) $(GUI) -o cut $(C++)
houghTest: hoghtTest.cpp
	$(CC) hoghtTest.cpp $(CV) $(CORE) $(GUI) $(PROC) -o houghTest $(C++)
brightfilter: brightfilter.cpp
	$(CC) brightfilter.cpp $(CV) $(CORE) $(GUI) $(PROC) -o brightfilter $(C++)

clean:
	rm -f challenge3 challenge3.o brightfilter.o find4corner.o transform.o cut.o
