#include "transform.h"

void helpOfTransform()
{
	printf("\n/*------------------------------------------------------------------------\n");
	printf(" This program is used to transform image to %d*%d.\n", imageSize, imageSize);
	printf(" intput: original image (Mat) and 4 corners of original image (Point [4])\n");
	printf(" output: a %d*%d square image (Mat)\n", imageSize, imageSize);
	printf(" It will save a result image named topic.jpg.\n");
	printf(" usage: ./transform [original image] [4-point] [result image]\n");
	printf(" 4-point: left-top(x,y) right-top(x,y) right-bottom(x,y) left-bottom(x,y)\n");
	printf(" result: a %d*%d square image\n", imageSize, imageSize);
	printf("------------------------------------------------------------------------*/\n");
}

/*
int main( int argc, char **argv )
{

	helpOfTransform();

	// load image	
	Mat src = imread(argv[1], 1);
	if(src.empty()){
		printf("Can not load image %s.\n", argv[1]);
		return -1;
	}
	
	// read 4 point
	vector<Point> p;
	for(int i=0; i<4; i++) p.push_back(Point(convertToInt(argv[i*2+2]), convertToInt(argv[i*2+3])));
	
	// transform
	Mat dst = transform(src, p);
	imwrite(argv[10], dst);
	
	// finished
	namedWindow("Origin", 1);
	namedWindow("Transformed", 1);
	imshow("Origin", src);
	imshow("Transformed", dst);
	waitKey(0);
	
	return 0;

}
*/

Mat transform(Mat src, vector<Point> p)
{
	
	printf("\nStart to transform to %d*%d ...\n", imageSize, imageSize);
	
	// set corners - before and after
	Point2f pts1[] = {Point2f(p[0].x, p[0].y), Point2f(p[1].x, p[1].y), Point2f(p[2].x, p[2].y), Point2f(p[3].x, p[3].y)};
	Point2f pts2[] = {Point2f(0,0), Point2f(imageSize-1, 0), Point2f(imageSize-1, imageSize-1), Point2f(0, imageSize-1)};
	
	// transform
	Mat dst = Mat(imageSize, imageSize, src.type());
	Mat M = getPerspectiveTransform(pts1, pts2);
	warpPerspective(src, dst, M, dst.size(), INTER_LINEAR);
	
	return dst;
	
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



