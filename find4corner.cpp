#include "find4corner.h"
#define kickpoint 300
#define kickdouble 0.005
#define threshold 40
#define WIDTH 2304
#define HEIGHT 1536

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
	printf(" intput: the photo taken by the camera (Mat) and dark image (Mat)\n");
	printf(" output: 4 corners (Point [4])\n");
	printf(" usage: ./find4corner [original] [dark] [result]\n");
	printf(" result: the image with edge points, edges, and 4 corners.\n");
	printf(" 4-point: left-top(x,y) right-top(x,y) right-bottom(x,y) left-bottom(x,y)\n");
	printf("------------------------------------------------------------------------*/\n");
}

/*
int main( int argc, char **argv )
{   

	helpOfFind4corner();

	// load image
	Mat src = imread(argv[1], 1);
	Mat dark = imread(argv[2], 0);
	if(src.empty() || dark.empty()){
		printf("Can not load image %s.\n", argv[1]);
		return -1;
	}
	
	// find 4 corners - harris
	/*Mat dst, dst_norm;
	cornerHarris(dark, dst, 2, 3, 0.04, BORDER_DEFAULT);
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	for(int j=0; j<dst_norm.rows; j++){
		for(int i=0; i<dst_norm.cols; i++){
			if((int)dst_norm.at<float>(j, i)>threshold){
				circle(src, Point(i, j), 5, colors[0], -1, 8, 0);
			}
		}
	}
	
	// find 4 edges - hough
	Mat mid, dst = src.clone();
	vector<Point2d> Line = findLineHough(dark, mid, dst);
	
	// find 4 edges - regression
	/*vector<Point2d> Line;
	for(int i=0; i<4; i++){
		Line.push_back(findLineRegression(dark, dst, i));
	}
	
	// find 4 corners
	vector<Point> corners;
	for(int i=0; i<4; i++){
		printf("\nsolve %d:\n", i);
		printf("\tL%d: y = %.2fx + %.2f\n", i, Line[i].x, Line[i].y);
		printf("\tL%d: y = %.2fx + %.2f\n", (i+3)%4, Line[(i+3)%4].x, Line[(i+3)%4].y);
		corners.push_back(findPointofTwoLine(Line[i].x, Line[i].y, Line[(i+3)%4].x, Line[(i+3)%4].y));
		//printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);
		circle(dst, corners[i], 5, Scalar(255,255,0), -1, 8, 0);
	}
	printf("\n");
	for(int i=0; i<4; i++) printf("corner[%d] = (%d,%d)\n", i, corners[i].x, corners[i].y);	
	
	// open window and show
	imshow("Origin", src);
	imshow("Result", dst);
	imwrite(argv[3], dst);
	waitKey(0);
	
	return 0;
	
}
*/

vector<Point2d> findLineHough(Mat src, Mat& mid, Mat& dst)
{
	
	printf("\nStart to find lines used hough ...\n");
	printf("image size: %d*%d\n", src.cols, src.rows);
	
	// find corners - hough
	vector<vector<Vec4i>> pointOfLine;
	vector<Vec4i> lines;
	vector<Point2d> Line;
	for(int i=0; i<4; i++) Line.push_back(Point2d(0, 0));
	
	// find lines and classify
	Canny(src, mid, 50, 200, 3);
	HoughLinesP(mid, lines, 1, CV_PI/180, 50, 50, 10 );
	printf("Result of hough transform: %d\n", (int)lines.size());
	pointOfLine = classifyLine(lines, src.rows, src.cols);
	for( int i=0; i<4; i++ ) {
		for(int j=0; j<pointOfLine[i].size(); j++){
			// print every lines
			Vec4i l = pointOfLine[i][j];
			double m = (double)(l[1]-l[3]) / (double)(l[0]-l[2]);
			double k1 = (double)l[1] - m*(double)l[0];
			double k2 = (double)l[3] - m*(double)l[2];
			double k = (k1+k2) / 2;
			line( dst, Point(l[0], l[1]), Point(l[2], l[3]), colors[i%9], 5, 8 );
			printf("L%d: y = %.2fx + %.2f\n", i, m, k);
    	}
	}
	
	// find 4 edges
	printf("Result of 4 edges:\n");
	for(int i=0; i<4; i++){
		if(pointOfLine[i].size()<=0){
			Line[i] = findLineRegression(src, dst, i);
		} else {
			for(int j=0; j<pointOfLine[i].size(); j++){
				double m = (double)(pointOfLine[i][j][1]-pointOfLine[i][j][3]) / (double)(pointOfLine[i][j][0]-pointOfLine[i][j][2]);
				double k1 = (double)pointOfLine[i][j][1] - m*(double)pointOfLine[i][j][0];
				double k2 = (double)pointOfLine[i][j][3] - m*(double)pointOfLine[i][j][2];
				Line[i].x += m;
				Line[i].y += (k1+k2);
			}
			Line[i].x /= pointOfLine[i].size();
			Line[i].y /= (pointOfLine[i].size()*2);
		}
		if(i%2==0) line( dst, Point(0, (int)Line[i].y), Point(dst.cols, (int)((double)dst.cols*Line[i].x+Line[i].y)), colors[5], 1, 8 );
		else line( dst, Point((int)(-Line[i].y/Line[i].x), 0), Point((int)(((double)dst.rows-Line[i].y)/Line[i].x), dst.rows), colors[5], 1, 8 );
		printf("L%d: y = %.2fx + %.2f\n", i, Line[i].x, Line[i].y);
	}
	
	return Line;
	
}

Point2d findLineRegression(Mat src, Mat& dst, int dir)
{
	
	printf("\nStart to find line used regression ...\n");
	printf("image size: %d*%d\n", src.cols, src.rows);
	
	// find corners - regression
	vector<Point> pointOfLine;
	Point2d Line = Point2d(0, 0);

	// finde line
	pointOfLine = collectData(src, dir);
	findLine(pointOfLine, Line.x, Line.y);
	printf("before:\n");
	if(dir%2==0) {
		line( dst, Point(0, (int)Line.y), Point(dst.cols, (int)((double)dst.cols*Line.x+Line.y)), colors[4], 1, 8 );
		printf("y = %.2fx + %.2f\n", Line.x, Line.y);
	} else {
		double a = (double)dst.cols;
		double b = (double)dst.rows;
		double am = a*Line.x;
		line( dst, Point((int)Line.y, 0), Point((int)(am+Line.y), dst.rows), colors[6], 1, 8 );
		printf("y = %.2fx + %.2f\n", b/am, -b*Line.y/am);
	}
	
	// kick out and recalculate
	kickOutDataPoint(pointOfLine, Line.x, Line.y);
	findLine(pointOfLine, Line.x, Line.y);
	printf("after:\n");
	printf("data point: %d\n", (int)pointOfLine.size());
	for(int j=0; j<pointOfLine.size(); j++){
		if(dir%2==0) circle(dst, pointOfLine[j], 2, colors[0], -1, 8, 0);
		else circle(dst, Point(pointOfLine[j].y, pointOfLine[j].x), 2, colors[2], -1, 8, 0);
	}
	if(dir%2==0){
		line( dst, Point(0, (int)Line.y), Point(dst.cols, (int)((double)dst.cols*Line.x+Line.y)), colors[7], 1, 8 );
	} else {
		double a = (double)dst.cols;
		double b = (double)dst.rows;
		double am = a*Line.x;
		Line.x = b/am;
		Line.y = -b*Line.y/am;
		line( dst, Point((int)((b-Line.y)/Line.x), dst.rows), Point((int)(-Line.y/Line.x), 0), colors[7], 1, 8 );
	}
	printf("y = %.2fx + %.2f\n", Line.x, Line.y);
	
	return Line;
	
}

vector<vector<Vec4i>> classifyLine(vector<Vec4i> lines, int r, int c)
{
	
	vector<vector<Vec4i>> Line;
	for(int i=0; i<4; i++){
		vector<Vec4i> l;
		Line.push_back(l);
	}
	
	// classify
	for(int i=0; i<lines.size(); i++){
		double m = (double)(lines[i][1]-lines[i][3]) / (double)(lines[i][0]-lines[i][2]);
		//printf("m[%d] = %.2f\n", i, m);
		if(isfinite(m)==0) continue;
		int dir;
		if(fabs(m)<0.2){
			if(lines[i][1]<r/2) dir = 0;
			else dir = 2;
		} else {
			if(lines[i][0]<c/2) dir = 3;
			else dir = 1;
		}
		Line[dir].push_back(lines[i]);
	}
	
	// establish table
	int table[WIDTH/10];
	vector<int> max;
	for(int i=0; i<4; i++){
		for(int j=0; j<WIDTH/10; j++) table[j] = 0;
		for(int j=0; j<Line[i].size(); j++){
			if(i%2){
				table[Line[i][j][0]/10]++;
				table[Line[i][j][2]/10]++;
			} else {
				table[Line[i][j][1]/10]++;
				table[Line[i][j][3]/10]++;
			}
		}
		max.push_back(find_max(table, i));
		//printf("max[%d] = %d\n", i, max[i]);	
	}
	
	// kick according table
	double kickline_x = (max[1]-max[3])/16;
	double kickline_y = (max[2]-max[0])/16;
	//printf("x: %.2f y: %.2f\n", kickline_x, kickline_y);
	for(int i=0; i<4; i++){
		if(i%2){
			for(int j=Line[i].size()-1; j>=0; j--){
				//printf("Line[%d][%d][0] = %d\n", i, j, Line[i][j][0]);
				if(fabs(Line[i][j][0]-max[i]) > kickline_x)
					Line[i].erase(Line[i].begin()+j);
			}
		} else {
			for(int j=Line[i].size()-1; j>=0; j--){
				//printf("Line[%d][%d][1] = %d\n", i, j, Line[i][j][1]);
				if(fabs(Line[i][j][1]-max[i]) > kickline_y)
					Line[i].erase(Line[i].begin()+j);
			}
		}
	}
	
	return Line;
	
}

int find_max(int* table, int dir)
{
	int max = 0, index;
	if(dir==0 || dir==3){
		index = 0;
		for(int i=0; i<WIDTH/10; i++){
			if(table[i]>max){
				max = table[i];
				index = i;
			}
		}
	} else if(dir==1 || dir==2){
		index = WIDTH/10-1;
		for(int i=WIDTH/10-1; i>=0; i--){
			if(table[i]>max){
				max = table[i];
				index = i;
			}
		}
	}
	return index*10+5;
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
	for(double j=1; kickpoint*j>30; j*=0.5){
		//printf("%d*%f=%f\n", kickpoint, j, kickpoint*j);
		for(int i=src.size()-1; i>=0; i--){
			double nu = fabs(m*src[i].x - src[i].y + k);
			double de = sqrt(m*m+1);
			if(nu/de > kickpoint*j) src.erase(src.begin()+i);
		}
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

vector<Point> collectData(Mat src, int dir)
{
	vector<Point> Line;
	
	if(dir==0){
		for(int i=0; i<src.cols; i++){
			for(int j=0; j<src.rows; j++){
				if(src.at<uchar>(j, i) == 255){
					Line.push_back(Point(i, j));
					break;
				}
			}
		}
	} else if(dir==1){
		for(int i=0; i<src.rows; i++){
			for(int j=src.cols-1; j>=0; j--){
				if(src.at<uchar>(i, j) == 255){
					Line.push_back(Point(i, j));
					break;
				}
			}
		}
	} else if(dir==2){
		for(int i=0; i<src.cols; i++){
			for(int j=src.rows-1; j>=0; j--){
				if(src.at<uchar>(j, i) == 255){
					Line.push_back(Point(i, j));
					break;
				}
			}
		}
	} else if(dir==3){
		for(int i=0; i<src.rows; i++){
			for(int j=0; j<src.cols; j++){
				if(src.at<uchar>(i, j) == 255){
					Line.push_back(Point(i, j));
					break;
				}
			}
		}
	}
	
	return Line;
}


