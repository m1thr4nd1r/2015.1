#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void showWindow(string name, IplImage* img)
{
	cvNamedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
	cvShowImage(name.c_str(), img);
	cvWaitKey(0);
}

int main(int argc, char **argv)
{
	// Input:
	// ../../imgs/cerebro.png
	IplImage * img = NULL;     //aloca
    if(argc > 1 && (img = cvLoadImage(argv[1],0)) != 0)
    {
		IplImage * res = img;
	    showWindow("coin", img);	    
	}
	destroyAllWindows();
    cvReleaseImage(&img);
}