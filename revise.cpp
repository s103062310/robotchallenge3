#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#define WIDTH 2304
#define HEIGHT 1536

using namespace std;
using namespace cv;

Mat src;
vector<Point> corners;

void onMouse(int event, int x, int y, int flags, void* para)
{
	if(event==CV_EVENT_LBUTTONDOWN) corners.push_back(Point(x, y));
}

int main(int argc, char **argv)
{
	
	// prepare
	string str(argv[1]);
	if(str=="camera"){
	
		// open camera
		printf("Opening camera ...\n");
		VideoCapture cap(0);
		cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
		if(!cap.isOpened()){
			printf("fail to open.\n");
			return -1;
		}
		cap >> src;

	} else {
	
		// load image
		printf("Image loading ...\n");
		src = imread(argv[1], 1);

	}
	
	if(src.empty()){
		printf("Can not load images.\n");
		return -1;
	}
	imwrite("src.jpg", src);
	
	// mouse event
	namedWindow("Output", CV_WINDOW_NORMAL);
	resizeWindow("Output", WIDTH*0.5, HEIGHT*0.5);
	setMouseCallback("Output", onMouse, 0);
	imshow("Output", src);
	
	// finish and show
	waitKey(0);
	fstream fp;
	fp.open("corner_file.txt", ios::out);
	if(!fp) cout << "Fail to open file!\n";
	for(int i=0; i<corners.size(); i++){
		fp << corners[i].x << " " << corners[i].y << " ";
		printf("Point[%d]: (%d,%d)\n", i, corners[i].x, corners[i].y);
	}
	fp.close();
	
	return 0;
	
}
