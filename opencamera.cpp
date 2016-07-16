#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(){

	// open camera
	VideoCapture cap(0);
	if(!cap.isOpened()){
		printf("fail to open.\n");
		return -1;
	}

	// open window and show
	Mat frame;
	char* filename = (char*)"#00.jpg";
	int num = 0;
	namedWindow("Webcam", 1);
	namedWindow("Photo", 1);
	while(true){
		
		cap>>frame;
		imshow("frame", frame);
		char key = waitKey(10);
		
		// press enter to take a snapshot; other to close program
		if(key==13){
			Mat photo = frame;
			imwrite(filename, photo);
			imshow("Photo", photo);
			num++;
			filename[1] = num/10 + '0';
			filename[2] = num%10 + '0';
		} else if(key>=0) break;
		
	}
	printf("finished.\n");

	return 0;

}
