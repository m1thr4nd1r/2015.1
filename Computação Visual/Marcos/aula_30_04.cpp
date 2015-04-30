#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	IplImage * img;
	if(argc > 1 && (img = cvLoadImage(argv[1], 1)) != 0){

		while(cvWaitKey(10) < 0) {
			cvShowImage(argv[1], img);
		}

		cvDestroyWindow(argv[1]);
		cvReleaseImage(&img);
	}

	return 0;
}
