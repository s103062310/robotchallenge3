#include "find4corner.h"

Scalar colors[] =
{
    {{0,0,255}},		// red
    {{0,128,255}},		// orange
    {{0,255,255}},		// yellow
    {{0,255,0}},		// green
    {{255,128,0}},		// light blue
    {{255,255,0}},		// cyan
    {{255,0,0}},		// blue
    {{255,0,255}},		// purple
    {{255,255,255}}		// white
}; 

void helpOfFind4corner()
{
	printf("\n/*------------------------------------------------------------------------\n");
	printf(" This program is used to find 4 corners of topic in scene.\n");
	printf(" intput: the photo taken by the camera (Mat)\n");
	printf(" output: 4 corners (Point [4])\n");
	printf(" usage: ./find4corner [original] [result 1] [result 2]\n");
	printf(" result 1: the image only have white and black.\n");
	printf(" result 2: the image with edge points, edges, and 4 corners.\n");
	printf(" 4-point: left-top(x,y) right-top(x,y) right-bottom(x,y) left-bottom(x,y)\n");
	printf("------------------------------------------------------------------------*/\n");
}

/*
int main( int argc, char **argv )  
{   

	help();

	// load image
	Mat src = imread(argv[1], 0);
	Mat dst = imread(argv[1], 1);
	if(src.empty() || dst.empty()){
		printf("Can not load image %s.\n", argv[1]);
		return -1;
	}
	
	// find corners
	vector<Point> corners = find4corner(src, dst);
	imwrite(argv[2], src);
	imwrite(argv[3], dst);
	for(int i=0; i<4; i++) printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);
	
	// open window and show
	namedWindow("Origin", 1);
	namedWindow("Result", 1);
	imshow("Origin", src);
	imshow("Result", dst);
	waitKey(0);
	
	return 0;
	
}
*/

void find4cornerHough(Mat src, Mat& mid, Mat& dst)
{
	
	printf("\nStart to find 4 corner ...\n");
	printf("image size: %d*%d\n", src.cols, src.rows);
	
	// find corners - hough
	vector<vector<Vec4i>> Line;
	double M[4] = {0.0, 0.0, 0.0, 0.0};
	double K[4] = {0.0, 0.0, 0.0, 0.0};
	vector<Point> corners;
	vector<Vec4i> lines;
	
	// find line
	Canny(src, mid, 50, 200, 3);
	HoughLinesP(mid, lines, 1, CV_PI/180, 50, 50, 10 );
	for( int i=0; i<lines.size(); i++ ) {
		Line = classifyLine(lines, src.rows, src.cols);
		Vec4i l = lines[i];
		double m = (double)(l[1]-l[3]) / (double)(l[0]-l[2]);
		double k1 = (double)l[1] - m*(double)l[0];
		double k2 = (double)l[3] - m*(double)l[2];
		double k = (k1+k2) / 2;
		//printf("lines %d: m = %.2f k1 = %.2f k2 = %.2f k = %.2f\n", i, m, k1, k2, k);
		line( dst, Point(0, (int)k), Point(dst.cols, (int)((double)dst.cols*m+k)), colors[i], 1, 8 );
    	//line( dst, Point(l[0], l[1]), Point(l[2], l[3]), colors[i], 3, 8 );
    	printf("L%d: y = %.2fx + %.2f\n", i, m, k);
	}
	for(int i=0; i<4; i++){
		printf("L%d:\n", i);
		for(int j=0; j<Line[i].size(); j++){
			printf("(%d,%d) ", Line[i][j][0], Line[i][j][1]);
			printf("(%d,%d) ", Line[i][j][2], Line[i][j][3]);
		}
		printf("\n");
	}
	for(int i=0; i<4; i++){
		for(int j=0; j<Line[i].size(); j++){
			double m = (double)(Line[i][j][1]-Line[i][j][3]) / (double)(Line[i][j][0]-Line[i][j][2]);
			double k1 = (double)Line[i][j][1] - m*(double)Line[i][j][0];
			double k2 = (double)Line[i][j][3] - m*(double)Line[i][j][2];
			M[i] += m;
			K[i] += (k1+k2);
		}
		M[i] /= Line[i].size();
		K[i] /= (Line[i].size()*2);
		line( dst, Point(0, (int)K[i]), Point(dst.cols, (int)((double)dst.cols*M[i]+K[i])), colors[5], 1, 8 );
		printf("L%d: y = %.2fx + %.2f\n", i, M[i], K[i]);
	}
}

vector<Point> find4cornerRegression(Mat& src, Mat& dst)
{
	
	printf("\nStart to find 4 corner ...\n");
	printf("image size: %d*%d\n", src.cols, src.rows);
	
	// find corners - regression
	vector<vector<Point>> Line;
	double M[4], K[4];
	vector<Point> corners;
	for(int i=0; i<4; i++){
		vector<Point> l;
		Line.push_back(l);
	}
	
	// finde line
	collectData(src, Line);
	printf("\nbefore:\n");
	for(int i=0; i<4; i++){
		printf("L%d: ", i);
		findLine(Line[i], M[i], K[i]);
		if(i%2==0) {
			//line( dst, Point(0, (int)K[i]), Point(dst.cols, (int)((double)dst.cols*M[i]+K[i])), colors[6], 1, 8 );
			printf("y = %.2fx + %.2f\n", M[i], K[i]);
		} else {
			double a = (double)dst.cols;
			double b = (double)dst.rows;
			double am = a*M[i];
			//line( dst, Point((int)K[i], 0), Point((int)(am+K[i]), dst.rows), colors[6], 1, 8 );
			printf("y = %.2fx + %.2f\n", b/am, -b*K[i]/am);
		}
	}
	
	// kick out and recalculate
	printf("\nafter:\n");
	for(int i=0; i<4; i++){
		printf("L%d: ", i);
		kickOutDataPoint(Line[i], M[i], K[i]);
		findLine(Line[i], M[i], K[i]);
		/*for(int j=0; j<Line[i].size(); j++){
			if(i%2==0) circle(dst, Line[i][j], 2, colors[4], -1, 8, 0);
			else circle(dst, Point(Line[i][j].y, Line[i][j].x), 2, colors[4], -1, 8, 0);
		}*/
		if(i%2==0) int a;//line( dst, Point(0, (int)K[i]), Point(dst.cols, (int)((double)dst.cols*M[i]+K[i])), colors[7], 1, 8 );
		else {
			double a = (double)dst.cols;
			double b = (double)dst.rows;
			double am = a*M[i];
			M[i] = b/am;
			K[i] = -b*K[i]/am;
			//line( dst, Point((int)((b-K[i])/M[i]), dst.rows), Point((int)(-K[i]/M[i]), 0), colors[7], 1, 8 );
		}
		printf("y = %.2fx + %.2f\n", M[i], K[i]);
	}
	
	// find corners
	for(int i=0; i<4; i++){
		printf("\nsolve %d:\n", i);
		printf("\tL%d: y = %.2fx + %.2f\n", i, M[i], K[i]);
		printf("\tL%d: y = %.2fx + %.2f\n", (i+3)%4, M[(i+3)%4], K[(i+3)%4]);
		corners.push_back(findPointofTwoLine(M[i], K[i], M[(i+3)%4], K[(i+3)%4]));
		//printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);
		//circle(dst, corners[i], 5, colors[5], -1, 8, 0);
	}
	
	return corners;
	
}

vector<vector<Vec4i>> classifyLine(vector<Vec4i> lines, int r, int c)
{
	
	vector<vector<Vec4i>> Line;
	for(int i=0; i<4; i++){
		vector<Vec4i> l;
		Line.push_back(l);
	}
	
	for(int i=0; i<lines.size(); i++){
		double m = (double)(lines[i][1]-lines[i][3]) / (double)(lines[i][0]-lines[i][2]);
		//printf("m[%d] = %.2f\n", i, m);
		int dir;
		if(fabs(m)<0.2){
			if(lines[i][1]<r/2) dir = 0;
			else dir = 2;
		} else {
			if(lines[i][0]<c/2) dir = 3;
			else dir = 1;
			/*int a = lines[i][0];
			lines[i][0] = lines[i][1];
			lines[i][1] = a;
			a = lines[i][2];
			lines[i][2] = lines[i][3];
			lines[i][3] = a;*/
		}
		Line[dir].push_back(lines[i]);
	}
	
	return Line;
	
}

void findLine(vector<Point>& src, double& m, double& k)
{
	if(src.size()<=0) return;
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
	int size = src.size();
	for(int i=size-1; i>=0; i--){
		double nu = fabs(m*src[i].x - src[i].y + k);
		double de = sqrt(m*m+1);
		if(nu/de > kickpoint) src.erase(src.begin()+i);
	}
}

double calSlope(vector<Point> src, double barx, double bary)
{
	vector<double> slope;
	int size = src.size()/2;
	while(size>0){
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

void collectData(Mat src, vector<vector<Point>>& Line)
{
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
}


