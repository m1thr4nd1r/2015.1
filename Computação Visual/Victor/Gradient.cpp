#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <cmath>
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

void showWindow(string name, IplImage* img)
{
	cvNamedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
	cvShowImage(name.c_str(), img);
}

int gradient(IplImage *img, int x, int y, int channels)
{
	if (channels == 1)
	// B&W
		return abs(CV_IMAGE_ELEM(img,uchar,x,y)-CV_IMAGE_ELEM(img,uchar,x+1,y)) + 
			   abs(CV_IMAGE_ELEM(img,uchar,x,y)-CV_IMAGE_ELEM(img,uchar,x,y+1));
   	else if (channels == 3)
   	// RGB
	   	return abs(CV_IMAGE_ELEM(img,uchar,x,y*3)-CV_IMAGE_ELEM(img,uchar,x+1,y*3)) + 
			   abs(CV_IMAGE_ELEM(img,uchar,x,y*3+1)-CV_IMAGE_ELEM(img,uchar,x+1,y*3+1)) + 
			   abs(CV_IMAGE_ELEM(img,uchar,x,y*3+2)-CV_IMAGE_ELEM(img,uchar,x+1,y*3+2)) + 
			   abs(CV_IMAGE_ELEM(img,uchar,x,y*3)-CV_IMAGE_ELEM(img,uchar,x,(y+1)*3)) + 
			   abs(CV_IMAGE_ELEM(img,uchar,x,y*3+1)-CV_IMAGE_ELEM(img,uchar,x,(y+1)*3+1)) + 
			   abs(CV_IMAGE_ELEM(img,uchar,x,y*3+2)-CV_IMAGE_ELEM(img,uchar,x,(y+1)*3+2));
}

IplImage* grad(IplImage *img, int mode, int limiar,int channels, int Lb, int Lg)
{
	IplImage *g = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,channels);
	if (mode == 1)
	{
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				if (channels == 1)
					CV_IMAGE_ELEM(g,uchar,i,j) = gradient(img,i,j,channels);
				else if (channels == 3)
				{
					CV_IMAGE_ELEM(g,uchar,i,j*3) = gradient(img,i,j,channels);
					CV_IMAGE_ELEM(g,uchar,i,j*3+1) = gradient(img,i,j,channels);
					CV_IMAGE_ELEM(g,uchar,i,j*3+2) = gradient(img,i,j,channels);
				}
	}
	else if (mode == 2)
	{
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				if (gradient(img, i,j,channels) >= limiar)
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = gradient(img,i,j,channels);
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = gradient(img,i,j,channels);
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = gradient(img,i,j,channels);
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = gradient(img,i,j,channels);
					}
				}
				else
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = CV_IMAGE_ELEM(img,uchar,i,j);
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = CV_IMAGE_ELEM(img,uchar,i,j*3);
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = CV_IMAGE_ELEM(img,uchar,i,j*3+1);
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = CV_IMAGE_ELEM(img,uchar,i,j*3+2);
					}
				}
	}
	else if (mode == 3)
	{
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				if (gradient(img, i,j,channels) >= limiar)
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = gradient(img,i,j,channels);
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = gradient(img,i,j,channels);
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = gradient(img,i,j,channels);
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = gradient(img,i,j,channels);
					}
				}
				else
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = Lb;
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = Lb;
					}
				}
	}
	else if (mode == 4)
	{
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				if (gradient(img, i,j,channels) >= limiar)
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = Lg;
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = Lg;
					}
				}
				else
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = CV_IMAGE_ELEM(img,uchar,i,j);
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = CV_IMAGE_ELEM(img,uchar,i,j*3);
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = CV_IMAGE_ELEM(img,uchar,i,j*3+1);
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = CV_IMAGE_ELEM(img,uchar,i,j*3+2);
					}
				}
	}
	else if (mode == 5)
	{
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				if (gradient(img, i,j,channels) >= limiar)
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = Lg;
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = Lg;
					}
				}
				else
				{
					if (channels == 1)
						CV_IMAGE_ELEM(g,uchar,i,j) = Lb;
					else if (channels == 3)
					{
						CV_IMAGE_ELEM(g,uchar,i,j*3) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+1) = 0;
						CV_IMAGE_ELEM(g,uchar,i,j*3+2) = Lb;
					}
				}
	}
	return g;
}

int main(int argc, char **argv)
{
	// Input:
	// channels limiar Lb Lg img
	// 1 100 255 128 ../imgs/cerebro.png
	IplImage * img = NULL;     //aloca
    if(argc > 5 && (img  = cvLoadImage(argv[5],atoi(argv[1])/3)) != 0)
    {
		IplImage * g1 = grad(img,1,atoi(argv[2]),atoi(argv[1]),atoi(argv[3]),atoi(argv[4]));
    	IplImage * g2 = grad(img,2,atoi(argv[2]),atoi(argv[1]),atoi(argv[3]),atoi(argv[4]));
    	IplImage * g3 = grad(img,3,atoi(argv[2]),atoi(argv[1]),atoi(argv[3]),atoi(argv[4]));
    	IplImage * g4 = grad(img,4,atoi(argv[2]),atoi(argv[1]),atoi(argv[3]),atoi(argv[4]));
    	IplImage * g5 = grad(img,5,atoi(argv[2]),atoi(argv[1]),atoi(argv[3]),atoi(argv[4]));
    	
    	if (atoi(argv[1]) == 1)
    	{
    		IplImage * res = join('h',30,img,5,g1,g2,g3,g4,g5);
    		while (cvWaitKey(5) < 0)
        	   	showWindow("Img / Gradient 1 / Gradient 2 / Gradient 3 / Gradient 4 / Gradient 5", res);
        	cvReleaseImage(&res);
	    }
    	else if (atoi(argv[1]) == 3)
    	    while (cvWaitKey(5) < 0)
	        {
	           	showWindow("Img", img);
		        showWindow("g1", g1);
		        showWindow("g2", g2);
		        showWindow("g3", g3);
		        showWindow("g4", g4);
		        showWindow("g5", g5);
	        }
    	destroyAllWindows();
        cvReleaseImage(&img);
        cvReleaseImage(&g1);
     	cvReleaseImage(&g2);
     	cvReleaseImage(&g3);
     	cvReleaseImage(&g4);
     	cvReleaseImage(&g5);           
    }
    return 0;
}