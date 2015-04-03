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

void limiarizar(IplImage* img, IplImage* bin, int limiter)
{
	int i,j,sum = 0;
	for (i = 0; i < img->height; i++)
		for (j = 0; j < img->width; j++)
		    sum += CV_IMAGE_ELEM(img,uchar,i,j);
	
	if (limiter < 0) limiter = sum / (img->height * img->width);	
		    
	for (i = 0; i < img->height; i++)
		for (j = 0; j < img->width; j++)
			if (CV_IMAGE_ELEM(img, uchar, i, j) > limiter)
				CV_IMAGE_ELEM(bin, uchar, i, j) = 255;
			else
			    CV_IMAGE_ELEM(bin, uchar, i, j) = 0;
}

void rotulateRecursivo(int i, int j, IplImage *img, IplImage *bin, int index)
{
	CV_IMAGE_ELEM(bin, int, i, j) = index;
	if (i+1 < img->width && CV_IMAGE_ELEM(img, uchar, i, j) == CV_IMAGE_ELEM(img, uchar, i+1, j) && CV_IMAGE_ELEM(bin, int, i+1, j) == -1)
		rotulateRecursivo(i+1,j,img,bin,index);
	if (j+1 < img->height && CV_IMAGE_ELEM(img, uchar, i, j) == CV_IMAGE_ELEM(img, uchar, i, j+1) && CV_IMAGE_ELEM(bin, int, i, j+1) == -1)
		rotulateRecursivo(i,j+1,img,bin,index);
	if (i-1 >= 0 && CV_IMAGE_ELEM(img, uchar, i, j) == CV_IMAGE_ELEM(img, uchar, i-1, j) && CV_IMAGE_ELEM(bin, int, i-1, j) == -1)
		rotulateRecursivo(i-1,j,img,bin,index);
	if (j-1 >= 0 && CV_IMAGE_ELEM(img, uchar, i, j) == CV_IMAGE_ELEM(img, uchar, i, j-1) && CV_IMAGE_ELEM(bin, int, i, j-1) == -1)
		rotulateRecursivo(i,j-1,img,bin,index);
}

void rotularizarRecursivo(IplImage *img, IplImage *bin, IplImage *res)
{
	int i,j, index = 0;
	for (i = 0; i < img->height; i++)
		for (j = 0; j < img->width; j++)
			CV_IMAGE_ELEM(bin, int, i, j) = -1;
			
	for (i = 0; i < img->height; i++) 
		for (j = 0; j < img->width; j++)
			if (CV_IMAGE_ELEM(bin, int, i, j) == -1)
			{
			    rotulateRecursivo(i,j,img,bin,index);
				index++;
			}

	for (i = 0; i < img->height; i++) 
	{
		for (j = 0; j < img->width; j++)				
		{
			cout << (int) CV_IMAGE_ELEM(bin,int,i,j) << " ";
			CV_IMAGE_ELEM(res, uchar, i, j) = ((double) (CV_IMAGE_ELEM(bin,int,i,j) * 255) / index);
		}
		cout << endl;
	}			
}

void rotulateIterativo(int i, int j, IplImage *img, IplImage *bin, int *index, int *vector)
{
        if ((i == 0 || CV_IMAGE_ELEM(bin, int, i-1, j) == 0) &&
            (j == 0 || CV_IMAGE_ELEM(bin, int, i, j-1) == 0))            
            CV_IMAGE_ELEM(bin,int,i,j) = ++(*index);            
        else if ((i > 0 && CV_IMAGE_ELEM(bin, int, i-1, j) > 0) && (j > 0 && CV_IMAGE_ELEM(bin, int, i, j-1) > 0))
        {
            if (CV_IMAGE_ELEM(bin, int, i-1, j) == CV_IMAGE_ELEM(bin, int, i, j-1))
                CV_IMAGE_ELEM(bin, int, i, j) = CV_IMAGE_ELEM(bin, int, i-1, j);
            else
            {
                CV_IMAGE_ELEM(bin, int, i, j) = min(CV_IMAGE_ELEM(bin, int, i-1, j), CV_IMAGE_ELEM(bin, int, i, j-1));                
                vector[max(CV_IMAGE_ELEM(bin, int, i-1, j), CV_IMAGE_ELEM(bin, int, i, j-1))] = CV_IMAGE_ELEM(bin, int, i, j);
            }
        }
        else
        {
        	if (i > 0 && j > 0)
        		CV_IMAGE_ELEM(bin, int, i, j) = max(CV_IMAGE_ELEM(bin, int, i-1, j),CV_IMAGE_ELEM(bin, int, i, j-1));        
        	else if (i > 0)
                CV_IMAGE_ELEM(bin, int, i, j) = CV_IMAGE_ELEM(bin, int, i-1, j);	
            else if (j > 0)
                CV_IMAGE_ELEM(bin, int, i, j) = CV_IMAGE_ELEM(bin, int, i, j-1);
        }
}

void rotularizarIterativo(IplImage *bin, IplImage *rotInt, IplImage *rot)
{
	int i = 0,j = 0, index = 0, *vec = NULL;
	for (i = 0; i < bin->height; i++)
		for (j = 0; j < bin->width; j++)
			CV_IMAGE_ELEM(rotInt, int, i, j) = (CV_IMAGE_ELEM(bin, uchar, i, j) == 255)? 1 : 0;
	
	vec = (int*) malloc(sizeof(int) * bin->width * bin->height);
	for (i = 0; i < max(bin->width,bin->height); i++)
	    vec[i] = 0;
			
	for (i = 0; i < bin->height; i++) 
		for (j = 0; j < bin->width; j++)
			if (CV_IMAGE_ELEM(rotInt, int, i, j) == 1)
				rotulateIterativo(i,j,bin,rotInt,&index,vec);

	int indexQnt = 0;
	for (i = 1; i < index; i++)
	    if (vec[i] == 0)
	        indexQnt++;

	int k = 0;
	for (i = 0; i < rotInt->height; i++) 
		for (j = 0; j < rotInt->width; j++)
		{
			k = CV_IMAGE_ELEM(rotInt, int, i, j);
			if (k > 0)
			{
			    while (vec[k] != 0)
			        k = vec[k];
			    CV_IMAGE_ELEM(rotInt, int, i, j) = k;			        		
			}
		}
	
	cout << indexQnt << endl;
	
	for (i = 0; i < bin->height; i++) 
	{
		for (j = 0; j < bin->width; j++)				
		{
			cout << (int) CV_IMAGE_ELEM(rotInt,int,i,j) << " ";
			CV_IMAGE_ELEM(rot, uchar, i, j) = ((double) (CV_IMAGE_ELEM(rotInt,int,i,j) * 255) / indexQnt);
		}
		cout << endl;
	}			
}
 
void showWindow(string name, IplImage* img)
{
	cvNamedWindow(name.c_str(), CV_WINDOW_AUTOSIZE);
	cvShowImage(name.c_str(), img);
}

int main(int argc, char ** argv)
{
    IplImage * img = NULL;     //aloca
    if(argc > 1 && (img  = cvLoadImage(argv[1],0)) != 0)
    {
        IplImage * bin = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
        IplImage * rot = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
        IplImage * rotInt = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
        limiarizar(img,bin,-1);
        rotularizarIterativo(bin,rotInt,rot);        

        IplImage * res = join('h',10,img,2,bin,rot);
        showWindow("Img / Bin / Rot", res);         
        cvWaitKey(0); 
        destroyWindow("Img / Bin / Rot");
        cvReleaseImage(&img);
        cvReleaseImage(&bin);        
        cvReleaseImage(&rotInt);
		cvReleaseImage(&rot);
        cvReleaseImage(&res);
    }
    return 0;
}
