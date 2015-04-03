#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

// Filtro 3x3

int main(int argc, char** argv)
{
	IplImage *img, *img2, *img3;
	int i = 0, j = 0;
	int sum;

	if (argc == 2 && (img = cvLoadImage(argv[1],0)) != 0)
	{
		img2 = cvCreateImage(cvSize(img->height, img->width), IPL_DEPTH_8U, 1);
		img3 = cvCreateImage(cvSize(img->height, img->width), IPL_DEPTH_8U, 1);

		for (i = 0; i < img->height; i++)
			for (j = 0; j < img->width; j++)
			{
				sum = (CV_IMAGE_ELEM(img,uchar,i,j) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i,j);
				sum += (CV_IMAGE_ELEM(img,uchar,i,j-1) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i,j-1);
				sum += (CV_IMAGE_ELEM(img,uchar,i,j+1) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i,j+1);

				// printf("%d\n", sum);
				
				sum += (CV_IMAGE_ELEM(img,uchar,i-1,j) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i-1,j);
				sum += (CV_IMAGE_ELEM(img,uchar,i-1,j-1) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i-1,j-1);
				sum += (CV_IMAGE_ELEM(img,uchar,i-1,j+1) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i-1,j+1);

				// printf("%d\n", sum);

				sum += (CV_IMAGE_ELEM(img,uchar,i+1,j) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i+1,j);
				sum += (CV_IMAGE_ELEM(img,uchar,i+1,j+1) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i+1,j+1);
				sum += (CV_IMAGE_ELEM(img,uchar,i+1,j-1) == 0)? 255 : CV_IMAGE_ELEM(img,uchar,i+1,j-1);

				// printf("%d %d\n", sum, sum / 9);

				CV_IMAGE_ELEM(img2,uchar,i,j) = sum / 9;

				sum = CV_IMAGE_ELEM(img,uchar,i,j);
				sum += CV_IMAGE_ELEM(img,uchar,i,j-1);
				sum += CV_IMAGE_ELEM(img,uchar,i,j+1);

				// printf("%d\n", sum);
				
				sum += CV_IMAGE_ELEM(img,uchar,i-1,j);
				sum += CV_IMAGE_ELEM(img,uchar,i-1,j-1);
				sum += CV_IMAGE_ELEM(img,uchar,i-1,j+1);

				// printf("%d\n", sum);

				sum += CV_IMAGE_ELEM(img,uchar,i+1,j);
				sum += CV_IMAGE_ELEM(img,uchar,i+1,j+1);
				sum += CV_IMAGE_ELEM(img,uchar,i+1,j-1);

				CV_IMAGE_ELEM(img3,uchar,i,j) = sum / 9;
			}
	
		cvNamedWindow("Image view", 1);
		cvShowImage("Image view",img3);
		cvNamedWindow("Image view2", 1);
		cvShowImage("Image view2",img2);
		cvWaitKey(0);
		cvDestroyWindow("Image view");
		cvReleaseImage(&img2);
		cvReleaseImage(&img);
	}
	return 0;
}