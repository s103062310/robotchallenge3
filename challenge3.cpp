#include <stdio.h>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#define THERESHOLD 50

using namespace std;

vector<int> detect_edge(IplImage* img, int tar, int ave)
{
	int l=0;
	bool edge=false;
	vector<int> vec;
	
	vec.push_back(0);
	for(int i=0; i<tar; i++){
    	
    	int pixel=0;
    	if(tar==img->width){
			for(int j=0; j<ave; j++){
				pixel += ((uchar*) img->imageData + j*img->widthStep )[i];
			}
		} else {
			for(int j=0; j<ave; j++){
				pixel += ((uchar*) img->imageData + i*img->widthStep )[j];
			}
		}
    	pixel /= ave;
    	
    	if(pixel<THERESHOLD){
    		pixel = 0;
    		if(edge==true) {
    			edge = false;
    			int m = (i-l)/2+l;
    			vec.push_back(m);
    		}
    	}else {
    		if(edge==false) {
    			edge=true;
    			l = i;
    		}
    	}
    	
    }
    
    vec.push_back(tar);
    return vec;
}

int main(int argc, char **argv)
{
	
	/**************
	* detect edge *
	**************/
	
	// load image
	IplImage* img = cvLoadImage("sobel.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	if( !img)
    {
        fprintf( stderr, "Can not load sobel.jpg\n" );
        exit(-1);
    }
    
    // detect edge locate
    vector<int> locate_x = detect_edge(img, img->width, img->height);
    vector<int> locate_y = detect_edge(img, img->height, img->width);
    
    // print info
    printf("X: %d\n", (int)locate_x.size());
    for(int i=0; i<locate_x.size(); i++){
    	printf("%d ", locate_x[i]);
    }
    printf("\nY: %d\n", (int)locate_y.size());
    for(int i=0; i<locate_x.size(); i++){
    	printf("%d ", locate_y[i]);
    }
    printf("\n");
	
	/************
	* cut image *
	************/
	
	// load image
	img = cvLoadImage(argv[1], 1);
	printf("width=%d, height=%d\n", img->width, img->height);

	/* open window
	cvNamedWindow("ORI_PICTURE", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("PICTURE", CV_WINDOW_AUTOSIZE);
	cvShowImage("ORI_PICTURE", img);
	*/

	// when press any key: show next small image which has been cut
	for(int i=0; i<locate_x.size()-1; i++){
		for(int j=0; j<locate_y.size()-1; j++){
			int C = locate_x[i+1]-locate_x[i], R = locate_y[j+1]-locate_y[j];
			//printf("Now = (%d,%d) %d*%d\n", locate_x[i], locate_y[j], C, R);
			IplImage* snapshot = cvCreateImage( cvSize(C, R), 8, 3 );
			cvSetImageROI( img, cvRect( locate_x[i], locate_y[j], C, R ) );
    			cvCopy( img, snapshot );
			//cvShowImage("PICTURE", snapshot);
			
			// TODO: object detect
			
			//cvWaitKey(0);
		}
	}

	// finished
	cvReleaseImage(&img);
	//cvDestroyWindow("PICTURE");
	//cvDestroyWindow("ORI_PICTURE");
    
    return 0;

}
