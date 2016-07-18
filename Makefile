challenge3: challenge3.cpp
	g++ challenge3.cpp -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -lopencv_highgui -lopencv_core -lopencv_imgproc -o challenge3 -std=c++11
#opencamera: opencamera.cpp
#	g++ opencamera.cpp -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -lopencv_highgui -lopencv_core -o opencamera -std=c++11
#bright: bright.cpp
#	g++ bright.cpp -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -lopencv_highgui -lopencv_core -lopencv_imgproc -o bright -std=c++11
clean:
	rm -f bright
