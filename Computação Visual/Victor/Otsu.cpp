#include <iostream>
#include <cstdarg>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

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

int otsu(IplImage* img)
{
	int sum = img->height * img->width;
	double M = 0;
	int *hist = (int*) calloc(256,sizeof(int));
	
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
		{
			hist[CV_IMAGE_ELEM(img,uchar,i,j)]++;
			M += CV_IMAGE_ELEM(img,uchar,i,j);
		}
	
	M /= sum;		
//	cout << "M: " << fixed << M << " sum: " << sum << endl; 
	
	double *p = (double*) calloc(256,sizeof(double));
	double Vt = 0;
	
	for (int i = 0; i < 256; i++)
	{
		p[i] = (double) hist[i] / sum;	
		Vt += pow((i - M),2) * p[i];	
	}
		
//	cout << "Vt: " << Vt << endl;
	
	double *Vb = (double*) calloc(256,sizeof(double));
	double *V = (double*) calloc(256,sizeof(double));
	double mt,w0 = 0,w1,u0,u1,qnt;
	int maxPos = -1;
	for (int i = 0; i < 256; i++)
	{
		mt = 0, w1 = 0, u0 = 0, u1 = 0, qnt = 0;
		w0 += p[i];
		w1 = 1 - w0;
		
		for (int j = 0; j <= i; j++)
			mt += j * p[j];
		
		u0 = (w0 == 0)? 0 : (double) mt / w0;
		u1 = (w1 == 0)? 0 : (double) (M - mt) / w1;
		Vb[i] = w0 * w1 * pow((u1 - u0),2);
		V[i] = Vb[i] / Vt;
//		cout << "H[" << i << "]: " << hist[i] << " | P[" << i << "]: " << p[i] << " | w0: " << w0 << " | w1: " << w1 << " | mt: " << mt << " | u0: " << u0 << " | u1: " << u1 << " | Vb[" << i << "]: " << Vb[i] << " | V[" << i << "]: " << V[i] << endl;				
		
		if (maxPos == -1 || V[i] > V[maxPos])
			maxPos = i;
	}
	
	return maxPos;	
}

void limiarizar(IplImage* img, IplImage* bin, int limiter)
{
	int i,j,sum = 0;
	for (i = 0; i < img->height; i++)
		for (j = 0; j < img->width; j++)
		    sum += CV_IMAGE_ELEM(img,uchar,i,j);
		    
	for (i = 0; i < img->height; i++)
		for (j = 0; j < img->width; j++)
			if (CV_IMAGE_ELEM(img, uchar, i, j) > limiter)
				CV_IMAGE_ELEM(bin, uchar, i, j) = 255;
			else
			    CV_IMAGE_ELEM(bin, uchar, i, j) = 0;
}

void showWindow(string name, IplImage* img)
{
	cvNamedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
	cvShowImage(name.c_str(), img);
}

int main(int argc, char** argv)
{
	IplImage* img;
	if (argc == 2 && (img = cvLoadImage(argv[1],0)) != 0)
	{
		IplImage * bin = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
		int limiter = otsu(img);
		cout << "Limiar: " << limiter << endl;
		limiarizar(img,bin,limiter);
		
		IplImage * res = join('h',30,img,1,bin);
        showWindow("Img / Binarizada (Com Limiar por Otsu)", res);         
        cvWaitKey(0); 
        destroyWindow("Img / Binarizada (Com Limiar por Otsu)");
        cvReleaseImage(&img);
        cvReleaseImage(&bin);
        cvReleaseImage(&res);
	}
	return 0;
}
