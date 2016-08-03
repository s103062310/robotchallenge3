#include "brightfilter.h"
#define brightscale 25
#define colorscale 85

void helpOfBrightfilter()
{
	printf("\n/*---------------------------------------------------\n");
	printf(" This program is used to filter the image by bright.\n");
	printf(" intput: the photo taken by the camera (Mat)\n");
	printf(" output: the image only have white and black (Mat)\n");
	printf(" usage: ./brightfilter [original] [result]\n");
	printf("---------------------------------------------------*/\n");
}



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
	Mat dst = filterByRightEdge(src);
	imwrite(argv[2], dst);
	
	// finished
	imshow("Origin", src);
	imshow("Filtered", dst);
	waitKey(0);
	return 0;

}


Mat filterByAverageBright(Mat src)
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
			if(src.at<uchar>(y, x) <= bright-brightscale) dst.at<uchar>(y, x) = 0;
			else dst.at<uchar>(y, x) = 255;
		}
	}
	
	return dst;
	
}

Mat filterByRightEdge(Mat src)
{
	
	printf("\nStart to filter ...\n");
	
	// find average color of right edge
	int B=0, G=0, R=0, p=0;
	for(int y=0; y<src.rows; y++){
		for(int x=src.cols*0.95; x<src.cols; x++){
			B += src.at<Vec3b>(y, x)[0];
			G += src.at<Vec3b>(y, x)[1];
			R += src.at<Vec3b>(y, x)[2];
			p++;
		}
	}
	B /= p;
	G /= p;
	R /= p;
	Vec3i ave = Vec3i(B, G, R);
	printf("(%d,%d,%d)\n", B, G, R);
	
	// create binary image
	Mat dst = Mat(src.rows, src.cols, CV_8UC1);
	for(int y=0; y<src.rows; y++){
		for(int x=0; x<src.cols; x++){
			Vec3i pixel = Vec3i(src.at<Vec3b>(y, x)[0], src.at<Vec3b>(y, x)[3], src.at<Vec3b>(y, x)[2]);
			double d = dis(pixel, ave);
			if(d > colorscale) dst.at<uchar>(y, x) = 255;
			else dst.at<uchar>(y, x) = 0;
		}
	}
	
	// obvious
	line(src, Point((int)(src.cols*0.95), 0), Point((int)(src.cols*0.95), src.rows), Scalar(0, 0, 255), 3, 8);
	
	return dst;	
	
}

double dis(Vec3i a, Vec3i b)
{
	int x2 = pow(a[0]-b[0], 2);
	int y2 = pow(a[1]-b[1], 2);
	int z2 = pow(a[2]-b[2], 2);
	double dis = sqrt(x2+y2+z2);
	return dis;
}




