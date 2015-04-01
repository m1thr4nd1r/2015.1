#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

#include "lib.h"

#define DEBUG false

int main(int argc, char ** argv){
    IplImage * img;	//aloca
	if(argc > 1 && (img  = cvLoadImage(argv[1],0)) != 0){

		int histogram[MAX_GRAY_BINS];
		int limiar = calcHistogram(img,histogram);

		IplImage * bin = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,1);
		threshold(img, bin, limiar, PRETO, BRANCO);

		IplImage * rotIt = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_32S,1);
		FILL_IPL(rotIt, int, 0);

		int cmp = labeled(bin, rotIt, 0, 0);

		IplImage * showIt = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,1);
		for (int i = 0; i < rotIt->height; ++i){
			for (int j = 0; j < rotIt->width; ++j){
				CV_IMAGE_ELEM(showIt, uchar, i, j) = (CV_IMAGE_ELEM(rotIt, int, i, j) * 255) / (cmp - 1);
			}
		}
		
		IplImage * rotRec = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_32S,1);
		FILL_IPL(rotRec, int, SEMROTULO);

		int cor = 1;
		for (int i = 0; i < rotRec->height; ++i){
			for (int j = 0; j < rotRec->width; ++j){
				if(CV_IMAGE_ELEM(rotRec, int, i, j) == SEMROTULO){
					labeledRecursive(bin, rotRec, i, j, cor, 0);
					cor++;
				}
			}
		}

		IplImage * showRec = cvCreateImage(Size(img->width, img->height),IPL_DEPTH_8U,1);
		for (int i = 0; i < rotRec->height; ++i){
			for (int j = 0; j < rotRec->width; ++j){
				CV_IMAGE_ELEM(showRec, uchar, i, j) = (CV_IMAGE_ELEM(rotRec, int, i, j) * 255) / (cor-1);
			}
		}

		if(DEBUG){
			cout << "-----------" << endl;
			cout << "DEBUG" << endl;
			cout << "-----------" << endl;
			cout << "limiar: " << limiar << ", labelIt: " << cmp << ", labelRec: " << cor << endl;
			cout << endl;
			cout << "Imagem BIN:" << endl;
			PRINTBIN_IPL(bin, uchar);
			cout << endl;
			cout << "Imagem rotulada iterativa:" << endl;
			PRINT_IPL(showIt, uchar);
			cout << endl;
			cout << endl;
			PRINT_IPL(rotIt, int);
			cout << endl;
			cout << endl;
			cout << "Imagem rotulada recursiva:" << endl;
			PRINT_IPL(showRec, uchar);
			cout << endl;
		}
			
		cvNamedWindow(argv[1], CV_WINDOW_AUTOSIZE); 						//aloca memoria
		cvShowImage(argv[1], join('h',10,img,3,bin,showIt,showRec));		//exibe imagem
		cvWaitKey(0);		            			 						//sleep
		cvDestroyWindow(argv[1]);					 						//desaloca memoria
		cvReleaseImage(&img);		    			 						//desaloca espaco da imagem
	}
	return 0;
}
