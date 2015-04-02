#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv)
{
	IplImage* img;
	if (argc == 2 && (img = cvLoadImage(argv[1],1)) != 0)
	{
		cvNamedWindow("Image view", 1);
		cvShowImage("Image view",img);
		cvWaitKey(0);
		cvDestroyWindow("Image view");
		cvReleaseImage(&img);
	}
	return 0;
}