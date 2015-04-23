#include <iostream>
#include <cstdarg>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

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

void showWindow(string name, IplImage* img)
{
	cvNamedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
	cvShowImage(name.c_str(), img);
}

void kmeans(IplImage *img, IplImage *means, int n)
{
	int k[n],minimo, minIndex, *sumK, *qntK, step = 0;
	bool change = false;
	IplImage * show = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);	

	for (int i = 0; i < n; i++)
		k[i] = 127.5 * i;
	
	do
	{
		sumK = (int*) calloc(n,sizeof(int));
		qntK = (int*) calloc(n,sizeof(int));

		step++;
		change = false;
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
			{
				minIndex = -1;
				minimo = INT_MAX;
				for (int l = 0; l < n; l++)
				{
					int temp = CV_IMAGE_ELEM(img, uchar, i, j) - k[l];
					temp *= temp;
					int t = CV_IMAGE_ELEM(img, uchar, i, j);
					//cout << "t: " << t << " | l: " << k[l] << " | Temp: " << temp << endl;
					if (temp < minimo)
					{	
						minIndex = l;
						minimo = temp;
						//cout << minIndex << " " << minimo << endl;
					}
				}
				sumK[minIndex] += CV_IMAGE_ELEM(img,uchar,i,j);
				qntK[minIndex]++;
				if (CV_IMAGE_ELEM(means, int, i, j) != k[minIndex])
				{
					change = true;
					CV_IMAGE_ELEM(means, int, i, j) = k[minIndex];
				}
			}

		for (int l = 0; l < n; l++)
		{
			double temp = (double) sumK[l] / qntK[l]; 
			k[l] = (int) temp;
		}

		cout << "Step: " << step << endl;
		while(cvWaitKey(10) < 0)
		{
			for (int i = 0; i < means->height; i++) 
				for (int j = 0; j < means->width; j++)				
					CV_IMAGE_ELEM(show, uchar, i, j) = CV_IMAGE_ELEM(means,int,i,j);
			showWindow("KMeans - Step", show);
		}
	}while(change);
}

int main(int argc, char ** argv)
{
	IplImage * img = NULL;     //aloca
    if(argc > 1 && (img  = cvLoadImage(argv[1],0)) != 0)
    {
        IplImage * intmeans = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
        IplImage * means = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
        
        kmeans(img,intmeans,3);

        // for (int i = 0; i < means->height; i++) 
		// {
		// 	for (int j = 0; j < means->width; j++)				
		// 	{
		// 		cout << (int) CV_IMAGE_ELEM(intmeans,int,i,j) << " ";
		// 		CV_IMAGE_ELEM(means, uchar, i, j) = CV_IMAGE_ELEM(intmeans,int,i,j);
		// 	}
		// 	cout << endl;
		// }

        // IplImage * res = join('h',10,img,1,means);
        // showWindow("Img / KMeans", res);         
        // cvWaitKey(0); 
        // destroyWindow("Img / KMeans");
        cvReleaseImage(&img);
        cvReleaseImage(&means);
        cvReleaseImage(&intmeans);        
    }
}