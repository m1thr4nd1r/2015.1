#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#define SQR(a) (a)*(a)

using namespace std;

int main(int argc, char **argv) {
	IplImage *esq, *dir, *disp, *res, *erro;
	int i, j, k, n, d, dist, distmin, avgerro;
	string file = argv[1];

	esq = cvLoadImage((file+"esq.ppm").c_str(), 1);
	dir = cvLoadImage((file+"dir.ppm").c_str(), 1);
	disp = cvLoadImage((file+"disp.pgm").c_str(), 0);
	res = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);
	erro = cvCreateImage(cvSize(esq->width,esq->height), IPL_DEPTH_8U, 1);

	avgerro=0; n=0;
	for(i=0; i < esq->height; i++) {
		for(j=0; j < esq->width; j++) {
			distmin = INT_MAX;
			d = 0;
			for(k=j; k < esq->width; k++) {
				dist = SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3)-CV_IMAGE_ELEM(dir,uchar,i,j*3)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+1)-CV_IMAGE_ELEM(dir,uchar,i,j*3+1)) + SQR(CV_IMAGE_ELEM(esq,uchar,i,k*3+2)-CV_IMAGE_ELEM(dir,uchar,i,j*3+2));
				if(dist < distmin) {
					distmin = dist;
					d = k-j;
				}
			}
			CV_IMAGE_ELEM(res,uchar,i,j) = d*8;
			if(CV_IMAGE_ELEM(disp,uchar,i,j)) {
				CV_IMAGE_ELEM(erro,uchar,i,j) = abs(CV_IMAGE_ELEM(disp,uchar,i,j)-CV_IMAGE_ELEM(res,uchar,i,j));
				avgerro += CV_IMAGE_ELEM(erro,uchar,i,j);
				n++;
			}
			else
				CV_IMAGE_ELEM(erro,uchar,i,j) = 0;
		}
	}
	printf("Erro médio: %lf\n", (double)avgerro/n);

	while(cvWaitKey(10) < 0 && argc == 2) {
		cvShowImage("Direita", dir);
		cvShowImage("Esquerda", esq);
		cvShowImage("Disparidade", disp);
		cvShowImage("Resultado", res);
		cvShowImage("Erro", erro);
	}
	cvDestroyAllWindows();
}

