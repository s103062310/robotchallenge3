#challenge3: challenge3.cpp
#	g++ challenge3.cpp -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -lopencv_highgui -lopencv_core -lopencv_imgproc -o challenge3 -std=c++11
#opencamera: opencamera.cpp
#	g++ opencamera.cpp -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -lopencv_highgui -lopencv_core -o opencamera -std=c++11
find4corner: find4corner.cpp
	g++ find4corner.cpp -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -lopencv_highgui -lopencv_core -lopencv_imgproc -o find4corner -std=c++11
