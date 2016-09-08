#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "brightfilter.h"
#include "find4corner.h"
#include "transform.h"
#include "cut.h" 
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#define WIDTH 2304
#define HEIGHT 1536

using namespace std;
using namespace cv;

void help()
{
	printf("\n/*------------------------------------------------------------------------\n");
	printf(" This program integrate find4corner, transform and cut.\n");
	printf(" intput: the photo taken by the camera (Mat)\n");
	printf(" output: 256 32*32 images (Mat)\n");
	printf(" It will save result images named cutXXX.jpg in cut_result.\n");
	printf(" usage: ./challenge3 [src]/camera [adjust]\n");
	printf(" dark: the image only have white and black.\n");
	printf(" showcorner: the image with edge points, edges, and 4 corners.\n");
	printf(" adjust: a 512*512 square image\n");
	printf("------------------------------------------------------------------------*/\n");
}

int main(int argc, char **argv)
{

	help();
	
	// prepare
	Mat src;
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
	
	/*/ blur
	Mat blur;
	medianBlur(src, blur, 47);
	
	// filter
	Mat dark = filterByRightEdge(blur);
	imwrite(argv[2], dark);
	
	// find 4 edges
	Mat mid, showcorner = src.clone();
	vector<Point2d> Line = findLineHough(dark, mid, showcorner);
	
	// find 4 corners
	vector<Point> corners;
	for(int i=0; i<4; i++){
		printf("\nsolve %d:\n", i);
		printf("\tL%d: y = %.2fx + %.2f\n", i, Line[i].x, Line[i].y);
		printf("\tL%d: y = %.2fx + %.2f\n", (i+3)%4, Line[(i+3)%4].x, Line[(i+3)%4].y);
		corners.push_back(findPointofTwoLine(Line[i].x, Line[i].y, Line[(i+3)%4].x, Line[(i+3)%4].y));
		//printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);
		circle(showcorner, corners[i], 5, Scalar(255,255,0), -1, 8, 0);
	}
	printf("\n");
	for(int i=0; i<4; i++) printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);
	imwrite(argv[3], showcorner);
	*/
	
	// get corners
	vector<Point> corners;
	char line[5];
	fstream fin;
	fin.open("corner_file.txt", ios::in);
	int n=0;
	while(fin.getline(line, sizeof(line), ' ')){
		int i=0, pixel=0;
		while(line[i]!='\0'){
			pixel *= 10;
			pixel += line[i]-'0';
			i++;
		}
		if(n%2==0) corners.push_back(Point(pixel, 0));
		else corners[n/2].y = pixel;
		n++;
	}
	
	// transform
	Mat adjust = transform(src, corners);
	imwrite(argv[2], adjust);
	
	// cut
	vector<Mat> dst = cut(adjust);
	system("mkdir cut_result");
	string filename("cut_result/cut000.jpg");
	for(int i=0; i<dst.size(); i++){
		filename[16] = i%10 + '0';
		filename[15] = i/10 % 10 + '0';
		filename[14] = i/100 % 10 + '0';
		imwrite(filename, dst[i]);
	}
	
	// TODO: object detect
	
	// finished
	printf("\n");
	imshow("Origin", src);
	//imshow("Dark", dark);
	//imshow("Edge", mid);
	//imshow("Corners", showcorner);
	imshow("Transformed", adjust);
	waitKey(0);
	
	return 0;
	
}
