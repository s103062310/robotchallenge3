#ifndef _CUT_H_
#define _CUT_H_

#include <cstdio>
#include <cstdlib>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#define imageSize 512
#define numOfImage 16
#define outSize 32

using namespace std;
using namespace cv;

vector<Mat> cut(Mat src);

#endif


