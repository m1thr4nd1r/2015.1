#include <iostream>
#include <cstdarg>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define SQR(a) (a)*(a)

// -----------------Codigo de Java-----------------

void copy(IplImage * src, IplImage * dst, int step, char flag){
	int i, j;
    for(i = 0; i < src->height; ++i){
		for(j = 0; j < src->width; ++j){
    		if(flag == 'v')
        		CV_IMAGE_ELEM(dst, uchar, step + i, j) = CV_IMAGE_ELEM(src, uchar, i, j);
        	else
        		CV_IMAGE_ELEM(dst, uchar, i, step + j) = CV_IMAGE_ELEM(src, uchar, i, j);
    	}
    } 
}

IplImage * join(char flag, int esp, IplImage * base, int n, ... ){

	IplImage * join;
	if(flag == 'h')
		join = cvCreateImage(cvSize(base->width * (n + 1) + esp * n, base->height), base->depth, base->nChannels);
	else
		join = cvCreateImage(cvSize(base->width, base->height * (n + 1) + esp  * n), base->depth, base->nChannels);

	copy(base, join, 0, flag);

	const int dim = (flag == 'h') ? base->width : base->height;
	int passo = (flag == 'h') ? base->width : base->height; 

	va_list args;
	va_start(args, n);
	for(int x = 0; x < n; x++){
        IplImage * im = va_arg(args, IplImage *);
        copy(im, join, passo + x * dim + esp * (x + 1), flag);
	}
    va_end(args);
	return join; 
}

// ------------------------------------------------

int main(int argc, char **argv) {
	IplImage *esq, *dir, *disp, *res, *erro;
	int i, j, k, n, d, dist, distmin, avgerro;
	string file = argv[1];

	esq = cvLoadImage((file+"esq.ppm").c_str(), 1);
	dir = cvLoadImage((file+"dir.ppm").c_str(), 1);
	disp = cvLoadImage((file+"disp.pgm").c_str(), 0);
//	esq = cvLoadImage(argv[1], 1);
//	dir = cvLoadImage(argv[2], 1);
//	disp = cvLoadImage(argv[3], 0);
	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	erro = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);

	avgerro=0; n=0;
	for(i=0; i < esq->height; i++) 
	{
		for(j=0; j < esq->width; j++) 
		{
			distmin = INT_MAX;
			d = 0;
			
			for(k=j; k < esq->width; k++) 
			{
				if ((CV_IMAGE_ELEM(esq,uchar,i,k*3)-CV_IMAGE_ELEM(esq,uchar,i,j*3) <= 0.2) &&
				   (CV_IMAGE_ELEM(esq,uchar,i,k*3+1)-CV_IMAGE_ELEM(esq,uchar,i,j*3+1) <= 0.2) &&
				   (CV_IMAGE_ELEM(esq,uchar,i,k*3+2)-CV_IMAGE_ELEM(esq,uchar,i,j*3+2) <= 0.2))
				   dist = CV_IMAGE_ELEM(esq,uchar,i,k*3)-CV_IMAGE_ELEM(esq,uchar,i,j*3) + CV_IMAGE_ELEM(esq,uchar,i,k*3+1)-CV_IMAGE_ELEM(esq,uchar,i,j*3+1) + CV_IMAGE_ELEM(esq,uchar,i,k*3+2)-CV_IMAGE_ELEM(esq,uchar,i,j*3+2);
				if(dist < distmin) 
				{
					distmin = dist;
					d = k-j;
				}
			}
			
//			-------------- Correspondencia por Cor mais proxima --------------
//			for(k=j; k < esq->width; k++) 
//			{
//				dist = SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3)-CV_IMAGE_ELEM(dir,uchar,i,j*3)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+1)-CV_IMAGE_ELEM(dir,uchar,i,j*3+1)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+2)-CV_IMAGE_ELEM(dir,uchar,i,j*3+2));
//				if(dist < distmin) 
//				{
//					distmin = dist;
//					d = k-j;
//				}
//			}
//			----------------------- Erro medio ~= 40 -------------------------
//			------------------------------------------------------------------
			
//			------------------------ Calculo do Erro -------------------------
			CV_IMAGE_ELEM(res,uchar,i,j) = d*8;
			if(CV_IMAGE_ELEM(disp,uchar,i,j)) 
			{
				CV_IMAGE_ELEM(erro,uchar,i,j) = abs(CV_IMAGE_ELEM(disp,uchar,i,j)-CV_IMAGE_ELEM(res,uchar,i,j));
				avgerro += CV_IMAGE_ELEM(erro,uchar,i,j);
				n++;
			}
			else
				CV_IMAGE_ELEM(erro,uchar,i,j) = 0;
//			------------------------------------------------------------------
		}
	}
	cout << "Erro médio: " << (double) avgerro/n << endl;
  
    IplImage * out = join('h',30,disp,2,res,erro);         

	while(cvWaitKey(10) < 0 && argc == 2) {
		cvShowImage("Direita", dir);
		cvShowImage("Esquerda", esq);
		cvShowImage("Disp / Res / Erro", out);         
		//cvShowImage("Disparidade", disp);
		//cvShowImage("Resultado", res);
		//cvShowImage("Erro", erro);
	}
	cvDestroyAllWindows();
}

