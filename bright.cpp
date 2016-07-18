#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <stdio.h>  
#define brightValue 60
  
using namespace std;  
using namespace cv;  

int main( int argc, char **argv )  
{   
	static Scalar colors[] =
    {
        {{0,0,255}},		// red
        {{0,128,255}},		// orange
        {{0,255,255}},		// yellow
        {{0,255,0}},		// green
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}},
        {{255,255,255}}
    };
	
	// load and create image
	Mat src = imread(argv[1], 0);
	if(!src.data){
		printf("Can not load %s\n", argv[1]);
		return -1;
	}
	printf("%d*%d\n", src.cols, src.cols);
	Mat dst = imread(argv[1], 1);
	
	// create dark image
	for(int y=0; y<src.rows; y++){
		for(int x=0; x<src.cols; x++){
			if(src.at<uchar>(y, x) <= brightValue) src.at<uchar>(y, x) = 0;
			else src.at<uchar>(y, x) = 255;
		}
	}
	
	// zoom
	int corners[4] = {src.rows, 0, 0, src.cols};
	for(int y=0; y<src.rows; y++){
		for(int x=0; x<src.cols; x++){
			if(src.at<uchar>(y, x) == 255){
				if(y<=corners[0]) corners[0] = y;
				if(x>=corners[1]) corners[1] = x;
				if(y>=corners[2]) corners[2] = y;
				if(x<=corners[3]) corners[3] = x;
			}
		}
	}
	
	// print info
	for(int i=0; i<4; i++){
		printf("Point%d: %d\n", i, corners[i]);
	}
	
	// create zoom image
	Mat zoom = Mat(dst, Rect(corners[3], corners[0], corners[1]-corners[3], corners[2]-corners[0]));
	imwrite("dark.jpg", zoom);
	
	// draw corners
	line( dst, Point(0, corners[0]), Point(dst.cols, corners[0]), colors[0], 1, 8 );
	line( dst, Point(corners[1], 0), Point(corners[1], dst.rows), colors[1], 1, 8 );
	line( dst, Point(0, corners[2]), Point(dst.cols, corners[2]), colors[2], 1, 8 );
	line( dst, Point(corners[3], 0), Point(corners[3], dst.rows), colors[3], 1, 8 );	
	
	// open window and show
	namedWindow("original", 1);
	namedWindow("changed", 1);
	namedWindow("zoom", 1);
	imshow("original", src);
	imshow("changed", dst);
	imshow("zoom", zoom);
	waitKey(0);
	
	return 0;
	
}  
 
