#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
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
                if (i > 0 && j > 0)
                    CV_IMAGE_ELEM(bin, int, i, j) = min(CV_IMAGE_ELEM(bin, int, i-1, j), CV_IMAGE_ELEM(bin, int, i, j-1));
                else if (i > 0)
                    CV_IMAGE_ELEM(bin, int, i, j) = CV_IMAGE_ELEM(bin, int, i-1, j);
                else if (j > 0)
                    CV_IMAGE_ELEM(bin, int, i, j) = CV_IMAGE_ELEM(bin, int, i, j-1);
                vector[max(CV_IMAGE_ELEM(bin, int, i-1, j), CV_IMAGE_ELEM(bin, int, i, j-1))] = CV_IMAGE_ELEM(bin, int, i, j);
            }
        }
        else        
            CV_IMAGE_ELEM(bin, int, i, j) = max(CV_IMAGE_ELEM(bin, int, i-1, j),CV_IMAGE_ELEM(bin, int, i, j-1));        
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
			printf("%d ", CV_IMAGE_ELEM(bin,int,i,j));
			CV_IMAGE_ELEM(res, uchar, i, j) = ((double) (CV_IMAGE_ELEM(bin,int,i,j) * 255) / index);
		}
		printf("\n");
	}			
}

void rotularizarIterativo(IplImage *img, IplImage *bin, IplImage *res)
{
	int i,j, index = 0, *vec = NULL;
	for (i = 0; i < img->height; i++)
		for (j = 0; j < img->width; j++)
			CV_IMAGE_ELEM(bin, int, i, j) = (CV_IMAGE_ELEM(img, uchar, i, j) == 255)? 1 : 0;
	
	vec = malloc(sizeof(int) * max(img->width,img->height));
	for (i = 0; i < max(img->width,img->height); i++)
	    vec[i] = 0;
			
	for (i = 0; i < img->height; i++) 
		for (j = 0; j < img->width; j++)
			if (CV_IMAGE_ELEM(bin, int, i, j) == 1)
				rotulateIterativo(i,j,img,bin,&index,vec);

	int indexQnt = 0;
	for (i = 1; i < index; i++)
	    if (vec[i] == 0)
	        indexQnt++;
	
	for (i = 0; i < img->height; i++) 
		for (j = 0; j < img->width; j++)
		    if (CV_IMAGE_ELEM(bin, int, i, j) > 0)
			{
			    int k = CV_IMAGE_ELEM(bin, int, i, j);
			    while (vec[k] != 0)
			        k = vec[k];
			    CV_IMAGE_ELEM(bin, int, i, j) = k;			        		
			}
	
	printf("%d\n",indexQnt);
	
	for (i = 0; i < img->height; i++) 
	{
		for (j = 0; j < img->width; j++)				
		{
			printf("%d ", CV_IMAGE_ELEM(bin,int,i,j));
            CV_IMAGE_ELEM(res, uchar, i, j) = ((double) (CV_IMAGE_ELEM(bin,int,i,j) * 255) / indexQnt);
		}
		printf("\n");
	}			
}

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

// void showBigWindow(int windows, IplImage* imgs[])
// {
//     IplImage *img = cvCreateImage(cvSize(imgs[0]->width * windows + 10, imgs[0]->height), IPL_DEPTH_8U,1);
    
//     int i,j,k;
//     for (k = 0; k < windows; k++)
//         for (i = 0; i < imgs[k]->width; i++)
//             for (j = 0; j < imgs[k]->height; j++)
//     //            CV_IMAGE_ELEM(img, uchar, i + k * imgs[0]->width, j) = CV_IMAGE_ELEM(imgs[k], uchar, i, j);
        
//     cvNamedWindow("BG", CV_WINDOW_AUTOSIZE);
// 	cvShowImage("BG", img);
// 	cvWaitKey(0);                   
// 	cvDestroyWindow("BG");
// 	cvReleaseImage(&img);
// }

void showWindow(char *name, IplImage* img)
{
	cvNamedWindow(name, CV_WINDOW_AUTOSIZE);
	cvShowImage(name, img);
}

void destroyWin(char *name, IplImage *img)
{
	cvDestroyWindow(name);
	cvReleaseImage(&img);
}

int main(int argc, char ** argv)
{
    IplImage * img;     //aloca
    if(argc > 1 && (img  = cvLoadImage(argv[1],0)) != 0)
    {
        IplImage * bin = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
        IplImage * rotchar = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_8U,1);
        IplImage * rot = cvCreateImage(cvSize(img->width, img->height),IPL_DEPTH_32S,1);
        limiarizar(img,bin,-1);
        rotularizarRecursivo(bin,rot,rotchar);        

        showWindow("Normal",img);
        showWindow("Binarizada",bin);
        showWindow("Rotulado",rotchar);        
        cvWaitKey(0);                   
        destroyWin("Normal",img);
        destroyWin("Binarizada",bin);
        destroyWin("Rotulado",rotchar);
    }
    return 0;
}
