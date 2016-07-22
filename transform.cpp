#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int convertToInt(char* str);

void help()
{
	printf("\n/*------------------------------------------------------------------------\n");
	printf(" This program is used to transform image to 640*640.\n");
	printf(" intput: original image (Mat) and 4 corners of original image (Point [4])\n");
	printf(" output: a 640*640 topic image (Mat)\n");
	printf(" It will save a result image named topic.jpg.\n");
	printf(" usage: ./transform [original image] [8-point] [result image]\n");
	printf("------------------------------------------------------------------------*/\n\n");
}

int main( int argc, char **argv )
{

	help();

	// load image	
	Mat src = imread(argv[1], 1);
	if(src.empty()) return -1;
	int p[8];
	for(int i=0; i<8; i++) p[i] = convertToInt(argv[i+2]);
	
	// set corners - before and after
	Point2f pts1[] = {Point2f(p[0], p[1]), Point2f(p[2], p[3]), Point2f(p[4], p[5]), Point2f(p[6], p[7])};
	Point2f pts2[] = {Point2f(0,0), Point2f(639, 0), Point2f(639, 639), Point2f(0, 639)};
	
	// transform
	Mat dst = Mat(640, 640, src.type());
	Mat M = getPerspectiveTransform(pts1, pts2);
	warpPerspective(src, dst, M, dst.size(), INTER_LINEAR);
	
	// finished
	namedWindow("Original", 1);
	namedWindow("Warped", 1);
	imshow("Original", src);
	imshow("Warped", dst);
	imwrite(argv[10], dst);
	waitKey(0);
	return 0;

}

int convertToInt(char* str)
{
	int num=0, i=0;
	while(str[i]!='\0'){
		num *= 10;
		num += str[i]-'0';
		i++;
	}
	return num;
}



