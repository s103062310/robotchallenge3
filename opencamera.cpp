#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(){
	
	/*CvCapture *cap;
	IplImage *frame;
	int i=0;
	
	cvNamedWindow("Webcam", 1);
	
	while(i<10){
		cap = cvCaptureFromCAM(i);
		frame = cvQueryFrame(cap);
		cvShowImage("Webcam", frame);
		cvWaitKey(0);
		i++;
	} 
	
	cvReleaseCapture(&cap);
	cvDestroyWindow("window");*/
	
	VideoCapture cap(0);
	if(!cap.isOpened()){
		printf("fail to open.\n");
		return -1;
	}
	
	Mat frame;
	namedWindow("Webcam", 1);
	for(;;){
		cap>>frame;
		imshow("frame", frame);
		if(waitKey(30)>=0) break;
	}
	printf("finished.\n");

	return 0;

}
