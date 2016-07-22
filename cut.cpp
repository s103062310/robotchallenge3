#include <cstdio>
#include <cstdlib>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#define numOfImage 16
#define imageSize 32

using namespace std;
using namespace cv;

void help()
{
	printf("\n/*------------------------------------------------------------------------\n");
	printf(" This program is used to cut 512*512 image into 32*32 pieces.\n");
	printf(" intput: a 512*512 image (Mat)\n");
	printf(" output: 256 32*32 images (Mat)\n");
	printf(" It will save result images named cutXXX.jpg in cut_result.\n");
	printf(" usage: ./cut [original image]\n");
	printf("------------------------------------------------------------------------*/\n\n");
}

int main(int argc, char **argv){

	help();
	
	// load image
	Mat src = imread(argv[1], 1);
	printf("%d*%d\n", src.cols, src.rows);

	// open window
	namedWindow("Original", 1);
	namedWindow("Result", 1);
	imshow("Original", src);

	// when press any key: show next small image which has been cut
	system("mkdir cut_result");
	string filename("cut_result/cut000.jpg");
	for(int y=0; y<numOfImage; y++){
		for(int x=0; x<numOfImage; x++){
			int num = y*numOfImage + x;
			filename[16] = num%10 + '0';
			filename[15] = num/10 % 10 + '0';
			filename[14] = num/100 % 10 + '0';
			//printf("Now = (%d,%d)\n", x*imageSize, y*imageSize);
			Mat dst = Mat(src, Rect(x*imageSize, y*imageSize, imageSize, imageSize));
			imshow("Result", dst);
			imwrite(filename, dst);
			//waitKey(0);
		}
	}

	// finished	
	return 0;

}
