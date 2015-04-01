#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

#include "lib.h"

int main(int argc, char ** argv){
    IplImage * img;	//aloca
	if(argc > 1 && (img  = cvLoadImage(argv[1],0)) != 0){

		int histogram[MAX_GRAY_BINS];
		int limiar = calcHistogram(img,histogram);

		IplImage * bin = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,1);
		threshold(img, bin, limiar, PRETO, BRANCO);

		IplImage * rot = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_32S,1);
		FILL_IPL(rot, int, SEMROTULO);

		int cor = 0;
		for (int i = 0; i < rot->height; ++i){
			for (int j = 0; j < rot->width; ++j){
				if(CV_IMAGE_ELEM(rot, int, i, j) == SEMROTULO){
					labeledRecursive(bin, rot, i, j, cor, 0);
					cor++;
				}
			}
		}	

		IplImage * show4 = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,1);
		for (int i = 0; i < rot->height; ++i){
			for (int j = 0; j < rot->width; ++j){
				int px = CV_IMAGE_ELEM(rot, int, i, j);
				CV_IMAGE_ELEM(show4, uchar, i, j) = (px * 255) / cor;
			}
		}

		IplImage * rot2 = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_32S,1);
		FILL_IPL(rot2, int, SEMROTULO);

		cor = 0;
		for (int i = 0; i < rot2->height; ++i){
			for (int j = 0; j < rot2->width; ++j){
				if(CV_IMAGE_ELEM(rot2, int, i, j) == SEMROTULO){
					labeledRecursive(bin, rot2, i, j, cor, 1);
					cor++;
				}
			}
		}

		IplImage * show8 = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,1);
		for (int i = 0; i < rot->height; ++i){
			for (int j = 0; j < rot->width; ++j){
				int px = CV_IMAGE_ELEM(rot, int, i, j);
				CV_IMAGE_ELEM(show8, uchar, i, j) = (px * 255) / cor;
			}
		}

		cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE); 						//aloca memoria
		cvShowImage(argv[1], join('h',10,img,3,bin,show4,show8));			//exibe imagem
		cvWaitKey(0);		            			 						//sleep
		cvDestroyWindow(argv[1]);					 						//desaloca memoria
		cvReleaseImage(&img);		    			 						//desaloca espaco da imagem
	}
	return 0;
}
