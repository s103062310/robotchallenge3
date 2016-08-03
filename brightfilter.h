#ifndef _BRIGHTFILTER_H_
#define _BRIGHTFILTER_H_

#include <cstdio>
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

Mat filterByAverageBright(Mat src);
Mat filterByRightEdge(Mat src);
double dis(Vec3i a, Vec3i b);

#endif


