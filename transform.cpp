#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

//int convertToInt(char* str);

int main( int argc, char **argv )
{

	// load image	
	Mat src = imread("#00.jpg", 1);
	if(src.empty()) return -1;
	
	// set corners - before and after
	Point2f pts1[] = {Point2f(177, 10), Point2f(484, 25), Point2f(471, 325), Point2f(176, 326)};
	Point2f pts2[] = {Point2f(0,0), Point2f(479, 0), Point2f(479, 479), Point2f(0, 479)};
	
	// transform
	Mat dst;
	Mat M = getPerspectiveTransform(pts1, pts2);
	warpPerspective(src, dst, M, src.size(), INTER_LINEAR);
	
	// finished
	namedWindow("Original", 1);
	namedWindow("Warped", 1);
	imshow("Original", src);
	imshow("Warped", dst);
	waitKey(0);
	return 0;

}

/*int convertToInt(char* str)
{
	int num=0, i=0;
	while(str[i]!='\0'){
		num *= 10;
		num += str[i]-'0';
		i++;
	}
	return num;
}*/



