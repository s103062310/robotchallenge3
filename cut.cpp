#include "cut.h"
#define imageSize 512
#define numOfImage 16
#define outSize 32

void helpOfCut()
{
	printf("\n/*------------------------------------------------------------------------\n");
	printf(" This program is used to cut %d*%d image into 32*32 pieces.\n", imageSize, imageSize);
	printf(" intput: a %d*%d image (Mat)\n", imageSize, imageSize);
	printf(" output: %d %d*%d images (Mat)\n", numOfImage*numOfImage, outSize, outSize);
	printf(" It will save result images named cutXXX.jpg in cut_result.\n");
	printf(" usage: ./cut [original image]\n");
	printf("------------------------------------------------------------------------*/\n");
}

/*
int main(int argc, char **argv){

	helpOfCut();
	
	// load image
	Mat src = imread(argv[1], 1);
	if(src.empty()){
		printf("Can not load image %s.\n", argv[1]);
		return -1;
	}

	// cut
	vector<Mat> dst = cut(src);
	system("mkdir cut_result");
	string filename("cut_result/cut000.jpg");
	for(int i=0; i<dst.size(); i++){
		filename[16] = i%10 + '0';
		filename[15] = i/10 % 10 + '0';
		filename[14] = i/100 % 10 + '0';
		imwrite(filename, dst[i]);
	}
	
	return 0;

}
*/

vector<Mat> cut(Mat src)
{

	printf("\nStart to cut image ...\n");
	
	// cut
	vector<Mat> result;
	for(int y=0; y<numOfImage; y++){
		for(int x=0; x<numOfImage; x++){
			//printf("Now = (%d,%d)\n", x*outSize, y*outSize);
			result.push_back(Mat(src, Rect(x*outSize, y*outSize, outSize, outSize)));
		}
	}
	
	return result;

}



