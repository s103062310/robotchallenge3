#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <stdio.h>  
#define brightValue 80
  
using namespace std;  
using namespace cv;  

int main( int argc, char **argv )  
{   
	// load and create image
	Mat src = imread(argv[1], 0);
	if(!src.data){
		printf("Can not load %s\n", argv[1]);
		return -1;
	}
	printf("%d*%d\n", src.cols, src.cols);
	Mat dst = Mat::zeros(src.size(), src.type());
	
	// create dark image
	for(int y=0; y<src.rows; y++){
		for(int x=0; x<src.cols; x++){
			if(src.at<uchar>(y, x) <= brightValue) dst.at<uchar>(y, x) = 0;
			else dst.at<uchar>(y, x) = src.at<uchar>(y, x);
		}
	}
	
	// open window and show
	namedWindow("original", 1);
	namedWindow("changed", 1);
	imshow("original", src);
	imshow("changed", dst);
	waitKey(0);
	
	return 0;
	
}  
 
