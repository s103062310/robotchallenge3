#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(){

	CvCapture* cap = cvCaptureFromCAM(0);;
	IplImage* frame;

	// open window
	cvNamedWindow("Webcam", 1);

    // show content of camera
	while(true){
		frame = cvQueryFrame(cap);
		cvShowImage("Webcam", frame);
		if(cvWaitKey(0)) break;
	}

    // finished
    cvReleaseImage(&frame);
	cvReleaseCapture(&cap);
	cvDestroyWindow("window");

	/*VideoCapture cap(0);
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

	return 0;*/

}
