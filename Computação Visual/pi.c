/* Programa: Exibe uma imagem */
#include <stdio.h>
#include <opencv/highgui.h> 
#include <opencv/cv.h>

int main(int argc, char** argv){

	IplImage *img;
	IplImage *binaria;
	IplImage *rotulada;
	int i,j,w,h;

	if (argc == 2 && (img = cvLoadImage(argv[1],0)) != 0){
		h = img->height;
		w = img->width;
		IplImage *binaria = cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,1);
		IplImage *rotulada = cvCreateImage(cvSize(w,h),IPL_DEPTH_32S,1);

		for (i=0; i<h; i++)
			for (j=0; j<w; j++){
				if (CV_IMAGE_ELEM(img,uchar,i,j) > 127)
					CV_IMAGE_ELEM(binaria,uchar,i,j) = 255;
				else 
					CV_IMAGE_ELEM(binaria,uchar,i,j) = 0;
			}

		cvNamedWindow("Image view",1); // aloca memória para saída
		cvShowImage("Image view",binaria); // exibe uma imagem
		cvWaitKey(0);                     // sleep
		cvDestroyWindow("Image view");  // desaloca memória para janela
		cvReleaseImage(&img);    //desaloca espaço de imagem
		cvReleaseImage(&binaria);
		cvReleaseImage(&rotulada);
	}
	return 0;

}


