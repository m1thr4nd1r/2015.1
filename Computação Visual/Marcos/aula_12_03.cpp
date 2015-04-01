#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cstdarg>
#include <sstream>
#include "lib.h"

int main(int argc, char ** argv){
    IplImage * img;	//aloca
	if(argc > 1 && (img  = cvLoadImage(argv[1],0)) != 0){
		int f = 11;
		if(argc >= 3) f = strToInt(argv[2]) - '\0';

		IplImage * imgMedia = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
		averageBlur(img, imgMedia, f);

		IplImage * imgMaskMedia = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
		diff(img, imgMedia, imgMaskMedia);

		IplImage * imgMedian = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
		medianBlur(img, imgMedian, f);

		IplImage * imgMaskMedian = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
		diff(img, imgMedian, imgMaskMedian);

		IplImage * black = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);

		IplImage * res1 = join('h', 10, img, 2, imgMedia, imgMaskMedia);
		IplImage * res2 = join('h', 10, img, 2, imgMedian, imgMaskMedian);
		// IplImage * res3 = join('h', 10, imgMedia2, 2, black, imgMedian2);

		cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE); 						//aloca memoria
		cvShowImage(argv[1], join('v', 10, res1, 1, res2));	//exibe imagem
		cvWaitKey(0);		            			 						//sleep
		cvDestroyWindow(argv[1]);					 						//desaloca memoria

		cvReleaseImage(&imgMaskMedia);
		cvReleaseImage(&imgMaskMedian);
		cvReleaseImage(&imgMedian);
		cvReleaseImage(&imgMedia);
		cvReleaseImage(&img);		    			 						//desaloca espaco da imagem
	}
	return 0;
}
