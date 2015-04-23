#include <iostream>
#include <cstdarg>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

#define sqr(a) (a)*(a)

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

IplImage* kmeansColor(IplImage *img, int n, bool hide)
{
	int k[n][3],minimo, minIndex, sumK[n][3], *qntK, step = 0;
	bool change = false;
	IplImage * show = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,3);	
	IplImage * means = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
        
	for (int i = 0; i < n; i++)
	{
		// k[i] = 127.5 * i;
		// double temp = 255 * (i/n);
		k[i][0] = k[i][1] = k[i][2] = 127.5 * i;
		cout << "(" << k[i][0] << " " << k[i][1] << " " << k[i][2] << ") ";
	}
	
	do
	{
		qntK = (int*) calloc(n,sizeof(int));
		for (int i = 0; i < n; i++)
			sumK[i][0] = sumK[i][1] = sumK[i][2] = 0;
			
		step++;
		change = false;
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
			{
				minIndex = -1;
				minimo = INT_MAX;
				for (int l = 0; l < n; l++)
				{
					int temp = sqrt(sqr(CV_IMAGE_ELEM(img, uchar, i, j*3) - k[l][0]) + 
							   		sqr(CV_IMAGE_ELEM(img, uchar, i, j*3+1) - k[l][1]) +
							   		sqr(CV_IMAGE_ELEM(img, uchar, i, j*3+2) - k[l][2]));
					if (temp < minimo)
					{	
						minIndex = l;
						minimo = temp;
					}
				}

				sumK[minIndex][0] += CV_IMAGE_ELEM(img,uchar,i,j*3);
				sumK[minIndex][1] += CV_IMAGE_ELEM(img,uchar,i,j*3+1);
				sumK[minIndex][2] += CV_IMAGE_ELEM(img,uchar,i,j*3+2);
				qntK[minIndex]++;

				if (CV_IMAGE_ELEM(means, int, i, j) != minIndex)
				{
					change = true;
					CV_IMAGE_ELEM(means, int, i, j) = minIndex;
				}
			}

		for (int l = 0; l < n; l++)
			for (int j = 0; j < 3; j++)
			{
				double temp = (double) sumK[l][j] / qntK[l]; 
				k[l][j] = (int) temp;
			}
		
		cout << "Step: " << step << endl;
		for (int i = 0; i < means->height; i++) 
			for (int j = 0; j < means->width; j++)
			{				
				CV_IMAGE_ELEM(show, uchar, i, j*3) = k[CV_IMAGE_ELEM(means,int,i,j)][0];
				CV_IMAGE_ELEM(show, uchar, i, j*3+1) = k[CV_IMAGE_ELEM(means,int,i,j)][1];
				CV_IMAGE_ELEM(show, uchar, i, j*3+2) = k[CV_IMAGE_ELEM(means,int,i,j)][2];
			}

		while(!hide && cvWaitKey(10) < 0)
			showWindow("KMeansColor - Step", show);
		
	}while(change);

	destroyAllWindows();
	return show;
}

IplImage* kmeans(IplImage *img, int n, bool hide)
{
	int k[n],minimo, minIndex, *sumK, *qntK, step = 0;
	bool change = false;
	IplImage * show = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);	
	IplImage * means = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
        
	for (int i = 0; i < n; i++)
	{
		k[i] = 127.5 * i;
		cout << k[i] << " ";
		//double temp = 255 * (i/n);
		//k[i] = (int) temp;
	}
	cout << endl;

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
					if (temp < minimo)
					{	
						minIndex = l;
						minimo = temp;
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
		for (int i = 0; i < means->height; i++) 
			for (int j = 0; j < means->width; j++)				
				CV_IMAGE_ELEM(show, uchar, i, j) = CV_IMAGE_ELEM(means,int,i,j);
		
		while(!hide && cvWaitKey(10) < 0)
			showWindow("KMeans - Step", show);
		
	}while(change);

	destroyAllWindows();
	return show;
}

int main(int argc, char ** argv)
{
	IplImage * img = NULL;     //aloca
    if(argc > 3 && (img  = cvLoadImage(argv[2],1)) != 0)
    // if(argc > 1 && (img  = cvLoadImage(argv[1],0)) != 0)
    {
    	bool hide = (argv[3][0] == 'f' || argv[3][0] == 'F')? true : false;
    	cout << "Centros: " << atoi(argv[1]) << " | Hide? " << hide << endl;
    	IplImage * meansColor = kmeansColor(img,atoi(argv[1]),hide);
        img = cvLoadImage(argv[2],0);
        IplImage * means = kmeans(img,atoi(argv[1]),hide);
        // IplImage * res = join('h',10,img,2,means,meansColor);
        
        while (cvWaitKey(5) < 0)
        {
	        showWindow("Img", img);
	     	showWindow("KMeans", means);
	        showWindow("KMeansColor", meansColor);
        }
        destroyAllWindows();
        cvReleaseImage(&img);
        cvReleaseImage(&means);
        cvReleaseImage(&meansColor);
        // cvReleaseImage(&res);
    }
}