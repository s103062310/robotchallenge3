#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <cstdio>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Mat transform(Mat src, vector<Point> p);
int convertToInt(char* str);

#endif

