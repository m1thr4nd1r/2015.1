#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv)
{
	IplImage* img;
	int i = 0, j = 0;
	int *hist;
	long sum = 0;
	if (argc == 2 && (img = cvLoadImage(argv[1],1)) != 0)
	{
		hist = calloc(256,sizeof(int));
		for (i = 0; i < img->height; i++)
			for (j = 0; j < img->width; j++)
				hist[CV_IMAGE_ELEM(img,uchar,i,j)]++;

		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{
				printf("(%d)%d ", 16*i+j, hist[i*16+j]);
				sum += hist[i*16+j];
			}
			printf("\n");
		}

		printf("\n%ld\n",sum);
		cvNamedWindow("Image view", 1);
		cvShowImage("Image view",img);
		cvWaitKey(0);
		cvDestroyWindow("Image view");
		cvReleaseImage(&img);
	}
	return 0;
}