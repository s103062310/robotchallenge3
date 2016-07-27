#ifndef _BRIGHTFILTER_H_
#define _BRIGHTFILTER_H_

#include <cstdio>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#define scale 25

using namespace std;
using namespace cv;

Mat filter(Mat src);

#endif


