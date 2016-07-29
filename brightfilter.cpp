#include "brightfilter.h"

void helpOfBrightfilter()
{
	printf("\n/*---------------------------------------------------\n");
	printf(" This program is used to filter the image by bright.\n");
	printf(" intput: the photo taken by the camera (Mat)\n");
	printf(" output: the image only have white and black (Mat)\n");
	printf(" usage: ./brightfilter [original] [result]\n");
	printf("---------------------------------------------------*/\n");
}


/*
int main(int argc, char **argv)
{

	helpOfBrightfilter();

	// load image
	Mat src = imread(argv[1], 1);
	if(src.empty()){
		printf("Can not load image %s.\n", argv[1]);
		return -1;
	}
	
	// filter
	Mat dst = filter(src);
	imwrite(argv[2], dst);
	
	// finished
	imshow("Origin", src);
	imshow("Filtered", dst);
	waitKey(0);
	return 0;

}
*/

Mat filter(Mat src)
{
	
	printf("\nStart to filter ...\n");
	
	// convert to gray
	cvtColor(src, src, CV_RGB2GRAY);
	
	// find average of bright
	int bright=0;
	for(int y=0; y<src.rows; y++){
		for(int x=0; x<src.cols; x++){
			bright += src.at<uchar>(y, x);
		}
	}
	bright /= src.cols*src.rows;
	printf("average of bright is %d\n", bright);
	
	// create binary image
	Mat dst = Mat(src.rows, src.cols, src.type());
	for(int y=0; y<src.rows; y++){
		for(int x=0; x<src.cols; x++){
			if(src.at<uchar>(y, x) <= bright-scale) dst.at<uchar>(y, x) = 0;
			else dst.at<uchar>(y, x) = 255;
		}
	}
	
	return dst;
	
}




