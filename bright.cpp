#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include <stdio.h>
#include <math.h>
#include <vector>
#define brightValue 70
#define cornerValue 150
#define kickpoint 10
#define kickdouble 0.2
  
using namespace std;  
using namespace cv; 

Scalar colors[] =
{
    {{0,0,255}},		// red
    {{0,128,255}},		// orange
    {{0,255,255}},		// yellow
    {{0,255,0}},		// green
    {{255,128,0}},
    {{255,255,0}},
    {{255,0,0}},		// blue
    {{255,0,255}},		// purple
    {{255,255,255}}
}; 

void findLine(vector<Point>& src, double& m, double& k);
void calAveragePoint(vector<Point> src, double& barx, double& bary);
double calSlope(vector<Point> src, double barx, double bary);
void kickOutDataPoint(vector<Point>& src, double m, double k);
double calAverageDouble(vector<double> src);
void kickOutDataDouble(vector<double>& src, double bar);
Point findPointofTwoLine(double m1, double k1, double m2, double k2);

int main( int argc, char **argv )  
{   

	// load image
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
	imwrite(argv[2], src);
	
	// find corners - regression
	vector<Point> Line[4];
	double M[4], K[4];
	Point corners[4];
	
	// collect data
	for(int i=0; i<src.cols; i++){
		for(int j=0; j<src.rows; j++){
			if(src.at<uchar>(j, i) == 255){
				Line[0].push_back(Point(i, j));
				break;
			}
		}
		for(int j=src.rows-1; j>=0; j--){
			if(src.at<uchar>(j, i) == 255){
				Line[2].push_back(Point(i, j));
				break;
			}
		}
	}
	for(int i=0; i<src.rows; i++){
		for(int j=0; j<src.cols; j++){
			if(src.at<uchar>(i, j) == 255){
				Line[3].push_back(Point(i, j));
				break;
			}
		}
		for(int j=src.cols-1; j>=0; j--){
			if(src.at<uchar>(i, j) == 255){
				Line[1].push_back(Point(i, j));
				break;
			}
		}
	}
	
	// finde line
	printf("\nbefore:\n");
	for(int i=0; i<4; i++){
		printf("L%d: ", i);
		findLine(Line[i], M[i], K[i]);
		if(i%2==0) {
			line( dst, Point(0, (int)K[i]), Point(dst.cols, (int)((double)dst.cols*M[i]+K[i])), colors[6], 1, 8 );
			printf("y = %.2fx + %.2f\n", M[i], K[i]);
		} else {
			M[i] *= -1;
			double am = dst.cols*M[i];
			line( dst, Point((int)((double)dst.cols*M[i]+K[i]), 0), Point((int)K[i], dst.rows), colors[6], 1, 8 );
			printf("y = %.2fx + %.2f\n", -dst.rows/am, dst.rows*(am+K[i])/am);
		}
	}
	
	// kick out and recalculate
	printf("\nafter:\n");
	for(int i=0; i<4; i++){
		printf("L%d: ", i);
		kickOutDataPoint(Line[i], M[i], K[i]);
		findLine(Line[i], M[i], K[i]);
		for(int j=0; j<Line[i].size(); j++){
			if(i%2==0) circle(dst, Line[i][j], 2, colors[4], -1, 8, 0);
			else circle(dst, Point(Line[i][j].y, Line[i][j].x), 2, colors[4], -1, 8, 0);
		}
		if(i%2==0) line( dst, Point(0, (int)K[i]), Point(dst.cols, (int)((double)dst.cols*M[i]+K[i])), colors[7], 1, 8 );
		else {
			double am = -dst.cols*M[i];
			M[i] = -dst.rows/am;
			K[i] = dst.rows*(am+K[i])/am;
			line( dst, Point((int)(((double)dst.rows-K[i])/M[i]), dst.rows), Point((int)(-K[i]/M[i]), 0), colors[7], 1, 8 );
		}
		printf("y = %.2fx + %.2f\n", M[i], K[i]);
	}
	
	for(int i=0; i<4; i++){
		printf("\nsolve %d:\n", i);
		printf("\tL%d: y = %.2fx + %.2f\n", i, M[i], K[i]);
		printf("\tL%d: y = %.2fx + %.2f\n", (i+3)%4, M[(i+3)%4], K[(i+3)%4]);
		corners[i] = findPointofTwoLine(M[i], K[i], M[(i+3)%4], K[(i+3)%4]);
		printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);
		circle(dst, corners[i], 5, colors[5], -1, 8, 0);
	}
	
	// find corners - harris
	/*Mat mid = Mat::zeros(src.size(), CV_8UC1);
	Mat norm, scaled;
	
	cornerHarris(src, mid, 2, 3, 0.04, BORDER_DEFAULT);
	normalize(mid, norm, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
	convertScaleAbs(norm, scaled);
	
	for(int y=0; y<norm.rows; y++){
		for(int x=0; x<norm.cols; x++){
			if( (int)norm.at<uchar>(y, x) > cornerValue){
				printf("Point: (%d,%d)\n", x, y);
				circle(dst, Point(x, y), 5, colors[0], -1, 8, 0);
				circle(src, Point(x, y), 5, colors[0], -1, 8, 0);
				circle(scaled, Point(x, y), 5, colors[0], -1, 8, 0);
			}
		}
	}*/
	
	// open window and show
	namedWindow("original", 1);
	namedWindow("changed", 1);
	//namedWindow("scaled", 1);
	imshow("original", src);
	imshow("changed", dst);
	//imshow("scaled", scaled);
	waitKey(0);
	
	return 0;
	
}  

void findLine(vector<Point>& src, double& m, double& k)
{
	double Ux, Uy;
	calAveragePoint(src, Ux, Uy);
	m = calSlope(src, Ux, Uy);
	k = Uy - m*Ux;
	//printf("(%.2f,%.2f) m = %.2f\n", Ux, Uy, m);
}
 
void calAveragePoint(vector<Point> src, double& barx, double& bary)
{
	int x=0, y=0;
	for(int i=0; i<src.size(); i++){
		x += src[i].x;
		y += src[i].y;
	}
	barx = x / src.size();
	bary = y / src.size();
}

void kickOutDataPoint(vector<Point>& src, double m, double k)
{
	for(int i=src.size()-1; i>=0; i--){
		double nu = fabs(m*src[i].x - src[i].y + k);
		double de = sqrt(m*m+1);
		if(nu/de > kickpoint) src.erase(src.begin()+i);
	}
}

double calSlope(vector<Point> src, double barx, double bary)
{
	vector<double> slope;
	int size = src.size()/2;
	while(size>10){
		for(int i=0; i<size; i++){
			double nu = src[i].y - src[src.size()-size+i].y;
			double de = src[i].x - src[src.size()-size+i].x;
			double m = nu/de;
			//printf("m%d = %.2f/%.2f = %.2f\n", i, nu, de, m);
			slope.push_back(m);
		}
		size /= 2;
	}
	double barm = calAverageDouble(slope);
	//printf("bar of m before kick out = %.2f\n", barm);
	kickOutDataDouble(slope, barm);
	barm = calAverageDouble(slope);
	//printf("bar of m after kick out = %.2f\n", barm);
	return barm;
}

double calAverageDouble(vector<double> src)
{
	double num=0;
	for(int i=0; i<src.size(); i++){
		num += src[i];
	}
	//printf("m = %.2f / %d\n", num, (int)src.size());
	return num/src.size();
}

void kickOutDataDouble(vector<double>& src, double bar)
{
	for(int i=src.size()-1; i>=0; i--){
		if(fabs(src[i]-bar) > kickdouble) src.erase(src.begin()+i);
	}
}

Point findPointofTwoLine(double m1, double k1, double m2, double k2)
{
	double x = (k2-k1) / (m1-m2);
	double y = (m2*k1 - m1*k2) / (m2-m1);
	return Point((int)x, (int)y);
}







