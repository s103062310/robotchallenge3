#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#define WIDTH 2304
#define HEIGHT 1536 

using namespace cv;
using namespace std;

void help(){
	printf("\n/*------------------------------------------------\n");
	printf(" This program is used to open camera.\n");
	printf(" You can ...\n");
	printf(" 1. press 'S' to take a snapshot.\n");
	printf(" 2. press 'ESC' to close the program.\n");
	printf(" The photo will store in the directory named photo.\n");
	printf(" usage: ./opencamera\n");
	printf("------------------------------------------------*/\n\n");
}

int main(){

	help();
	
	// fetch time and make a new directory
	tm* ptrnow;
	time_t local_time=0;
	time(&local_time);
	ptrnow = localtime(&local_time);
	cout << asctime(ptrnow) << endl << endl;
	system("mkdir photo");
	
	// open camera
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
	if(!cap.isOpened()){
		printf("fail to open.\n");
		return -1;
	}

	// open window and show
	Mat frame;
	char filename[15] = "photo/#00.jpg";
	int num = 0;
	namedWindow("Webcam", 1);
	namedWindow("Photo", 1);
	while(true){
		
		cap>>frame;
		imshow("Webcam", frame);
		char key = waitKey(10);
		
		// press S to take a snapshot; ESC to close program
		if(key=='S'||key=='s'){
			Mat photo = frame;
			imwrite(filename, photo);
			imshow("Photo", photo);
			num++;
			filename[7] = num/10 + '0';
			filename[8] = num%10 + '0';
		} else if(key==27) break;
		
	}
	printf("\nfinished.\n");

	return 0;

}
