#include <cstdio>
#include <cstdlib>
#include <vector>
#include "find4corner.h"
#include "transform.h"
#include "cut.h" 
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

void help()
{
	printf("\n/*------------------------------------------------------------------------\n");
	printf(" This program integrate find4corner, transform and cut.\n");
	printf(" intput: the photo taken by the camera (Mat)\n");
	printf(" output: 256 32*32 images (Mat)\n");
	printf(" It will save result images named cutXXX.jpg in cut_result.\n");
	printf(" usage: ./challenge3 [src] [dark] [showcorner] [adjust]\n");
	printf(" dark: the image only have white and black.\n");
	printf(" showcorner: the image with edge points, edges, and 4 corners.\n");
	printf(" adjust: a 512*512 square image\n");
	printf("------------------------------------------------------------------------*/\n");
}

int main(int argc, char **argv)
{

	help();
	
	// load image
	Mat src = imread(argv[1], 1);
	Mat dark = imread(argv[1], 0);
	Mat showcorner = imread(argv[1], 1);
	if(src.empty() || dark.empty() || showcorner.empty()){
		printf("Can not load image %s.\n", argv[1]);
		return -1;
	}
	
	// find corners
	vector<Point> corners = find4corner(dark, showcorner);
	imwrite(argv[2], dark);
	imwrite(argv[3], showcorner);
	printf("\n");
	for(int i=0; i<4; i++) printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);
	
	// transform
	Mat adjust = transform(src, corners);
	imwrite(argv[4], adjust);
	
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
	
	// finished
	printf("\n");
	namedWindow("Origin", 1);
	namedWindow("Dark", 1);
	namedWindow("Corners", 1);
	namedWindow("Transformed", 1);
	imshow("Origin", src);
	imshow("Dark", dark);
	imshow("Corners", showcorner);
	imshow("Transformed", adjust);
	waitKey(0);
	
	return 0;
	
}
