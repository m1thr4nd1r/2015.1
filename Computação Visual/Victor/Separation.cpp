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

// ---------------------- Bugado ---------------------- //
void sep(pair<int,int> point, int index, IplImage* img, IplImage* cat,int limiar)
{
	if (CV_IMAGE_ELEM(cat,int,point.first,point.second) == -1)
	{
		pair<int,int> p = point;
		CV_IMAGE_ELEM(cat,int,point.first,point.second) = index;
		int dist;

		if (point.first < cat->height-1 && CV_IMAGE_ELEM(cat,int,point.first+1,point.second) == -1)
		{	
			dist = 	abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3)-CV_IMAGE_ELEM(img,uchar,point.first+1,point.second*3)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+1)-CV_IMAGE_ELEM(img,uchar,point.first+1,point.second*3+1)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+2)-CV_IMAGE_ELEM(img,uchar,point.first+1,point.second*3+1));
			if (dist < limiar)
			{
				p.first++;
				sep(p,index,img,cat,limiar);
			}
		}
		if (point.first > 0 && CV_IMAGE_ELEM(cat,int,point.first-1,point.second) == -1)	
		{
			dist = 	abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3)-CV_IMAGE_ELEM(img,uchar,point.first-1,point.second*3)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+1)-CV_IMAGE_ELEM(img,uchar,point.first-1,point.second*3+1)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+2)-CV_IMAGE_ELEM(img,uchar,point.first-1,point.second*3+1));
			if (dist < limiar)
			{
				p.first--;
				sep(p,index,img,cat,limiar);
			}
		}
		if (point.second > 0 && CV_IMAGE_ELEM(cat,int,point.first,point.second-1) == -1)	
		{
			dist = 	abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3)-CV_IMAGE_ELEM(img,uchar,point.first,(point.second-1)*3)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+1)-CV_IMAGE_ELEM(img,uchar,point.first,(point.second-1)*3+1)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+2)-CV_IMAGE_ELEM(img,uchar,point.first,(point.second-1)*3+1));
			if (dist < limiar)
			{
				p.second--;
				sep(p,index,img,cat,limiar);
			}
		}
		if (point.second < cat->width-1 && CV_IMAGE_ELEM(cat,int,point.first,point.second+1) == -1)	
		{
			dist = 	abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3)-CV_IMAGE_ELEM(img,uchar,point.first,(point.second+1)*3)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+1)-CV_IMAGE_ELEM(img,uchar,point.first,(point.second+1)*3+1)) +
					abs(CV_IMAGE_ELEM(img,uchar,point.first,point.second*3+2)-CV_IMAGE_ELEM(img,uchar,point.first,(point.second+1)*3+1));
			if (dist < limiar)
			{
				p.second++;
				sep(p,index,img,cat,limiar);
			}
		}
	}
}

IplImage* separation(IplImage* img, int seeds, int limiar)
{
	pair<int,int> points[seeds];
	for (int i = 0; i < seeds; i++)
		points[i] = make_pair(rand() % img->height, rand() % img->width);

	IplImage* cat = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			CV_IMAGE_ELEM(cat,int,i,j) = -1;

	for (int i = 0; i < seeds; i++)
		sep(points[i],i,img,cat,limiar);

	IplImage* show = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,3);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
		{
			int temp = CV_IMAGE_ELEM(cat,int,i,j);
			if (temp != -1)
			{
				CV_IMAGE_ELEM(show,uchar,i,j*3) = CV_IMAGE_ELEM(img,uchar,points[temp].first,points[temp].second*3);
				CV_IMAGE_ELEM(show,uchar,i,j*3+1) = CV_IMAGE_ELEM(img,uchar,points[temp].first,points[temp].second*3+1);
				CV_IMAGE_ELEM(show,uchar,i,j*3+2) = CV_IMAGE_ELEM(img,uchar,points[temp].first,points[temp].second*3+2);
			}
			else
			{
				CV_IMAGE_ELEM(show,uchar,i,j*3) = 0;
				CV_IMAGE_ELEM(show,uchar,i,j*3+1) = 0;
				CV_IMAGE_ELEM(show,uchar,i,j*3+2) = 0;
			}
		}

	return show;
}
// ---------------------- Bugado ---------------------- //

IplImage* separationI(IplImage *img, int seeds, int limiar)
{
	IplImage* cat = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			CV_IMAGE_ELEM(cat,int,i,j) = -1;

	pair<int,int> points[seeds];
	for (int i = 0; i < seeds; i++)
	{
		points[i] = make_pair(rand() % img->height, rand() % img->width);
		CV_IMAGE_ELEM(cat,int,points[i].first,points[i].second) = i;		
	}

	int dist;
	bool change = true;
	do
	{
		change = false;
		for (int i = 0; i < cat->height; i++)
			for (int j = 0; j < cat->width; j++)
				if (CV_IMAGE_ELEM(cat,int,i,j) == -1)
				{
					if (i < cat->height-1 && CV_IMAGE_ELEM(cat,int,i+1,j) != -1)
					{	
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j*3)-CV_IMAGE_ELEM(img,uchar,i+1,j*3)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+1)-CV_IMAGE_ELEM(img,uchar,i+1,j*3+1)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+2)-CV_IMAGE_ELEM(img,uchar,i+1,j*3+1));
						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i+1,j);
							change = true;
						}
					}
					else if (i > 0 && CV_IMAGE_ELEM(cat,int,i-1,j) != -1)	
					{
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j*3)-CV_IMAGE_ELEM(img,uchar,i-1,j*3)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+1)-CV_IMAGE_ELEM(img,uchar,i-1,j*3+1)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+2)-CV_IMAGE_ELEM(img,uchar,i-1,j*3+1));
						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i-1,j);
							change = true;
						}
					}
					else if (j > 0 && CV_IMAGE_ELEM(cat,int,i,j-1) != -1)	
					{
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j*3)-CV_IMAGE_ELEM(img,uchar,i,(j-1)*3)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+1)-CV_IMAGE_ELEM(img,uchar,i,(j-1)*3+1)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+2)-CV_IMAGE_ELEM(img,uchar,i,(j-1)*3+1));
						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i,j-1);
							change = true;
						}
					}
					else if (j < cat->width-1 && CV_IMAGE_ELEM(cat,int,i,j+1) != -1)	
					{
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j*3)-CV_IMAGE_ELEM(img,uchar,i,(j+1)*3)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+1)-CV_IMAGE_ELEM(img,uchar,i,(j+1)*3+1)) +
								abs(CV_IMAGE_ELEM(img,uchar,i,j*3+2)-CV_IMAGE_ELEM(img,uchar,i,(j+1)*3+1));
						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i,j+1);
							change = true;
						}
					}
				}
	}while (change);

	IplImage* show = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,3);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
		{
			int temp = CV_IMAGE_ELEM(cat,int,i,j);
			if (temp != -1)
			{
				CV_IMAGE_ELEM(show,uchar,i,j*3) = CV_IMAGE_ELEM(img,uchar,points[temp].first,points[temp].second*3);
				CV_IMAGE_ELEM(show,uchar,i,j*3+1) = CV_IMAGE_ELEM(img,uchar,points[temp].first,points[temp].second*3+1);
				CV_IMAGE_ELEM(show,uchar,i,j*3+2) = CV_IMAGE_ELEM(img,uchar,points[temp].first,points[temp].second*3+2);
			}
			else
			{
				CV_IMAGE_ELEM(show,uchar,i,j*3) = 0;
				CV_IMAGE_ELEM(show,uchar,i,j*3+1) = 0;
				CV_IMAGE_ELEM(show,uchar,i,j*3+2) = 0;
			}
		}

	return show;
}

IplImage* separationIBW(IplImage *img, int seeds, int limiar)
{
	IplImage* cat = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			CV_IMAGE_ELEM(cat,int,i,j) = -1;

	pair<int,int> points[seeds];
	for (int i = 0; i < seeds; i++)
	{
		points[i] = make_pair(rand() % img->height, rand() % img->width);
		CV_IMAGE_ELEM(cat,int,points[i].first,points[i].second) = i;		
	}

	int dist;
	bool change = true;
	do
	{
		change = false;
		for (int i = 0; i < cat->height; i++)
			for (int j = 0; j < cat->width; j++)
				if (CV_IMAGE_ELEM(cat,int,i,j) == -1)
				{
					if (i < cat->height-1 && CV_IMAGE_ELEM(cat,int,i+1,j) != -1)
					{	
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j)-CV_IMAGE_ELEM(img,uchar,i+1,j));
								
						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i+1,j);
							change = true;
						}
					}
					if (i > 0 && CV_IMAGE_ELEM(cat,int,i-1,j) != -1)	
					{
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j)-CV_IMAGE_ELEM(img,uchar,i-1,j));
						
						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i-1,j);
							change = true;
						}
					}
					if (j > 0 && CV_IMAGE_ELEM(cat,int,i,j-1) != -1)	
					{
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j)-CV_IMAGE_ELEM(img,uchar,i,(j-1)));

						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i,j-1);
							change = true;
						}
					}
					if (j < cat->width-1 && CV_IMAGE_ELEM(cat,int,i,j+1) != -1)	
					{
						dist = 	abs(CV_IMAGE_ELEM(img,uchar,i,j)-CV_IMAGE_ELEM(img,uchar,i,(j+1)));

						if (dist < limiar)
						{
							CV_IMAGE_ELEM(cat,int,i,j) = CV_IMAGE_ELEM(cat,int,i,j+1);
							change = true;
						}
					}
				}
	}while (change);

	IplImage* show = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
		{
			int temp = CV_IMAGE_ELEM(cat,int,i,j);
			if (temp != -1)
				CV_IMAGE_ELEM(show,uchar,i,j) = CV_IMAGE_ELEM(img,uchar,points[temp].first,points[temp].second);
			else
				CV_IMAGE_ELEM(show,uchar,i,j) = 0;
		}

	return show;
}

int main(int argc, char **argv)
{
	// Input:
	// SeedsBW LimiarBW SeedsColor LimiarColor img
	IplImage * img = NULL;     //aloca
    if(argc > 5 && (img  = cvLoadImage(argv[5],0)) != 0)
    {
    	IplImage * bw = separationIBW(img,atoi(argv[1]), atoi(argv[2]));
        img = cvLoadImage(argv[5],1);
    	IplImage * color = separationI(img,atoi(argv[3]), atoi(argv[4]));
    	
        while (cvWaitKey(5) < 0)
        {
	        showWindow("Img", img);
	        showWindow("Separation - B and W", bw);
	     	showWindow("Separation - Color", color);
        }
        destroyAllWindows();
        cvReleaseImage(&img);
        cvReleaseImage(&bw);
        cvReleaseImage(&color);
    }
    return 0;
}